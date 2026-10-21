/*
 * A220727  Number of ways to reciprocally link elements of an 3Xn array
 *          either to themselves or to exactly two horizontal, diagonal or
 *          antidiagonal neighbors.
 * https://oeis.org/A220727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence a(n) = 5*a(n-1) + 24*a(n-2) - a(n-3) - 25*a(n-4) -
 *            33*a(n-5) +10*a(n-6) + 19*a(n-7) + 4*a(n-8) - 4*a(n-9) - a(n-10)
 *            Initial values: a(1)=1, a(2)=4, a(3)=30, a(4)=230, a(5)=1819,
 *            a(6)=14466, a(7)=114900, a(8)=913187, a(9)=7256380,
 *            a(10)=57664692
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* realloc, abort */

#define INIT_TERMS 10

static const seq_t init_vals[INIT_TERMS] = {
    (seq_t)1, (seq_t)4, (seq_t)30, (seq_t)230, (seq_t)1819,
    (seq_t)14466, (seq_t)114900, (seq_t)913187, (seq_t)7256380, (seq_t)57664692
};

static const int coeffs[INIT_TERMS] = {
    5, 24, -1, -25, -33, 10, 19, 4, -4, -1
};

static seq_t *cache = NULL;          /* 1‑indexed cache */
static idx_t   cache_cap = 0;         /* highest index allocated */
static idx_t   last_computed = 0;     /* highest index already computed */

/* Ensure cache can hold indices up to n and initialise the first
   INIT_TERMS terms if not yet done. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap) return;               /* already allocated */

    idx_t new_cap = n;
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) abort();                       /* out of memory */

    /* Zero new slots (cache[0] unused). */
    for (idx_t i = cache_cap + 1; i <= new_cap; ++i) tmp[i] = 0;

    cache = tmp;
    cache_cap = new_cap;

    /* Initialise the first INIT_TERMS terms if not already done. */
    if (last_computed < INIT_TERMS) {
        for (idx_t i = 1; i <= INIT_TERMS && i <= cache_cap; ++i)
            cache[i] = init_vals[i - 1];
        last_computed = INIT_TERMS;
    }
}

/* Compute the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)            /* safety for the unlikely call with zero */
        return 0;

    if (n <= INIT_TERMS)   /* use pre‑computed initial values */
        return init_vals[n - 1];

    /* Allocate cache up to n. */
    ensure_cache(n);

    /* Compute any terms that haven't been computed yet. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (int k = 0; k < INIT_TERMS; ++k) {
            seq_t term = cache[i - 1 - k];
            int c = coeffs[k];
            if (c >= 0)
                sum += term * (seq_t)c;
            else
                sum -= term * (seq_t)(-c);
        }
        cache[i] = sum;
    }

    last_computed = n;
    return cache[n];
}
