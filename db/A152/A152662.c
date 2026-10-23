/*
 * A152662  Triangle read by rows: T(n,k) is the number of permutations of
 *          {1,2,...,n} for which k is the maximal number of initial odd
 *          entries (0 <= k <= ceiling(n/2)).
 * https://oeis.org/A152662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(row,k) where the flattened index n satisfies n =
 *            offset_row(row)+k. T(row,k) is the number of permutations of
 *            {1,…,row} whose longest initial run of odd numbers has length k.
 *            For row r = 2m (even): T(r,k) = m * k! * (2m-1-k)! *
 *            binomial(m,k) For row r = 2m+1 (odd, r≥3): T(r,k) = m * k! *...
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 62, u64 n <= 130,
 *            u128 n <= 340, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- factorial cache ---------- */
static seq_t *fact_cache = NULL;
static idx_t fact_len   = 0;

/* ensure factorials up to 'up' are present in the cache */
static void ensure_factorial(idx_t up)
{
    if (up < fact_len) return;
    idx_t new_len = up + 1;
    seq_t *new_cache = (seq_t *)realloc(fact_cache, sizeof(seq_t) * new_len);
    if (!new_cache) abort();                /* allocation failure */

    if (fact_len == 0) {                    /* first allocation */
        new_cache[0] = (seq_t)1;
        for (idx_t i = 1; i <= up; ++i)
            new_cache[i] = new_cache[i-1] * (seq_t)i;
    } else {                                /* extend existing cache */
        for (idx_t i = fact_len; i <= up; ++i)
            new_cache[i] = new_cache[i-1] * (seq_t)i;
    }

    fact_cache = new_cache;
    fact_len   = new_len;
}

/* retrieve factorial of n */
static seq_t factorial(idx_t n)
{
    if (n >= fact_len) ensure_factorial(n);
    return fact_cache[n];
}

/* multiplicative binomial coefficient */
static seq_t binom_value(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;          /* exact division */
    }
    return res;
}

/* compute T(row, k) for any row >= 0 */
static seq_t compute_T(idx_t row, idx_t k)
{
    if (row == (idx_t)0) return (seq_t)1;
    if (row == (idx_t)1) return (k == (idx_t)0) ? (seq_t)0 : (seq_t)1;

    if ((row & (idx_t)1) == (idx_t)0) {                  /* even row */
        idx_t m = row >> 1;                             /* m = row/2   */
        ensure_factorial(2 * m - 1);                     /* for later    */
        seq_t res = (seq_t)m;
        res = res * factorial(k);                       /* k!          */
        res = res * factorial(2 * m - 1 - k);            /* (2m-1-k)!   */
        res = res * binom_value(m, k);                  /* binomial(m,k) */
        return res;
    } else {                                            /* odd row (r>=3) */
        idx_t m = (row - 1) >> 1;                       /* m = (row-1)/2 */
        ensure_factorial(2 * m);                         /* for later    */
        seq_t res = (seq_t)m;
        res = res * factorial(k);                       /* k!          */
        res = res * factorial(2 * m - k);               /* (2m-k)!     */
        res = res * binom_value(m + 1, k);               /* binomial(m+1,k) */
        return res;
    }
}

/* ---------- main function required by the problem statement ---------- */
seq_t a(idx_t n)
{
    idx_t row = 0;
    idx_t cum = 0;                                 /* cumulative terms */

    /* find the row that contains the flattened index n */
    while (1) {
        idx_t size = ((row & (idx_t)1) ?
                      ((row >> 1) + 2) : /* odd row length */
                      ((row >> 1) + 1)); /* even row length */
        if (n < cum + size) break;
        cum += size;
        ++row;
    }

    idx_t k = n - cum;                               /* column within the row */
    return compute_T(row, k);
}
