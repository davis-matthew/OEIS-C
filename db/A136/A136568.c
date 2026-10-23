/*
 * A136568  Irregular triangle read by rows: row n contains the distinct
 *          values (in numerical order) making up the nonzero exponents in the
 *          prime factorization of n. (Row 1 = (0).).
 * https://oeis.org/A136568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For each index n find the row r of the triangle and the
   position k within that row, factor r, collect the distinct non‑zero
   exponents, sort them and return the k‑th exponent. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static idx_t *cum = NULL;            /* cumulative counts S(r) = Σ_{i=1}^r len(i) */
static size_t cum_cap = 0;           /* allocated capacity of cum */
static idx_t last_row = 0;           /* largest row whose cumulative count is stored */

/* ensure the cum array has room for at least `need` elements (0‑based index) */
static void grow_cum_to(size_t need)
{
    if (need > cum_cap) {
        size_t new_cap = cum_cap ? cum_cap * 2 : 64;
        if (new_cap < need) new_cap = need;
        idx_t *tmp = (idx_t *)realloc(cum, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        cum = tmp;
        cum_cap = new_cap;
    }
}

/* compute the number of distinct non‑zero exponents in the prime factorisation
   of x (x>=1).  For x==1 the result is 1 (the single element 0 in row 1). */
static size_t row_len_of(uint64_t x)
{
    if (x == 1) return 1;
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(x, primes, exps, &cnt);

    uint64_t uniq[64];
    size_t u = 0;
    for (size_t i = 0; i < cnt; ++i) {
        int found = 0;
        for (size_t j = 0; j < u; ++j)
            if (exps[i] == uniq[j]) { found = 1; break; }
        if (!found) uniq[u++] = exps[i];
    }
    return u;
}

/* gather the distinct exponents of x (x>=2), store them sorted in uniq[0..u-1] */
static void gather_exps(uint64_t x, uint64_t uniq[], size_t *u_out)
{
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(x, primes, exps, &cnt);

    size_t u = 0;
    for (size_t i = 0; i < cnt; ++i) {
        int found = 0;
        for (size_t j = 0; j < u; ++j)
            if (exps[i] == uniq[j]) { found = 1; break; }
        if (!found) uniq[u++] = exps[i];
    }

    /* insertion sort – u is small (≤ number of distinct primes, usually < 20) */
    for (size_t i = 1; i < u; ++i) {
        uint64_t temp = uniq[i];
        size_t j = i;
        while (j > 0 && uniq[j - 1] > temp) {
            uniq[j] = uniq[j - 1];
            --j;
        }
        uniq[j] = temp;
    }
    *u_out = u;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* safety for out‑of‑range calls */

    /* initialise cumulative table on first call */
    if (!cum) {
        size_t init_cap = 64;
        cum = (idx_t *)calloc(init_cap, sizeof(idx_t));
        if (!cum) abort();
        cum_cap = init_cap;
        cum[0] = 0;
        last_row = 0;
    }

    /* extend cumulative table until it covers index n */
    while (cum[last_row] < n) {
        ++last_row;
        grow_cum_to(last_row + 1);
        size_t len = row_len_of((uint64_t)last_row);
        cum[last_row] = cum[last_row - 1] + (idx_t)len;
    }

    idx_t r = last_row;                /* the row that contains a(n) */
    idx_t term_idx = n - cum[r - 1] - 1; /* 0‑based position in that row */

    if (r == 1) return (seq_t)0;       /* row 1 contains only 0 */

    uint64_t uniq[64];
    size_t u;
    gather_exps((uint64_t)r, uniq, &u);

    /* term_idx is guaranteed < u by construction of the cumulative table */
    return (seq_t)uniq[term_idx];
}
