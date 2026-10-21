/*
 * A211500  Number of -3..3 arrays x(i) of n+1 elements i=1..n+1 with
 *          x(i)+x(j), x(i+1)+x(j+1), -(x(i)+x(j+1)), and -(x(i+1)+x(j))
 *          having two or three distinct values for every i<=n and j<=n.
 * https://oeis.org/A211500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) +13*a(n-2) -43*a(n-3) -67*a(n-4) +254*a(n-5)
 *            +171*a(n-6) -797*a(n-7) -213*a(n-8) +1427*a(n-9) +85*a(n-10)
 *            -1458*a(n-11) +60*a(n-12) +810*a(n-13) -58*a(n-14) -224*a(n-15)
 *            +12*a(n-16) +24*a(n-17) with initial values a(1)=48, a(2)=130,
 *            a(3)=298, a(4)=660, a(5)=1384, a(6)=2890, a(7)=5...
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 57, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for memoisation */
static seq_t *cache = NULL;          /* array of computed terms (1‑based)    */
static idx_t  cache_size = 0;        /* allocated size of cache             */
static idx_t  max_computed = 0;      /* largest n computed so far           */

/* coefficients of the linear recurrence */
static const int coeff[17] = {
    3, 13, -43, -67, 254, 171, -797, -213,
    1427, 85, -1458, 60, 810, -58, -224, 12, 24
};

/* initial 17 terms of the sequence */
static const seq_t init_vals[17] = {
    (seq_t)48,    (seq_t)130,  (seq_t)298,  (seq_t)660,  (seq_t)1384,
    (seq_t)2890,  (seq_t)5850, (seq_t)11930,(seq_t)23806,(seq_t)48056,
    (seq_t)95422, (seq_t)191866,(seq_t)380806,(seq_t)765090,
    (seq_t)1521180,(seq_t)3058882,(seq_t)6098874
};

/* Grow the cache array so that it can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    idx_t new_cap = (cache_size == 0) ? 18 : cache_size;
    while (new_cap <= n) new_cap *= 2;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);  /* out of memory */

    for (idx_t i = 0; i < new_cap; ++i) new_cache[i] = (seq_t)0;
    if (cache) {
        for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_size = new_cap;
}

/* initialise the first 17 terms once */
static void init_terms(void)
{
    if (max_computed >= 17) return;

    if (!cache) ensure_cache(17);

    for (idx_t i = 1; i <= 17; ++i) {
        cache[i] = init_vals[i - 1];
    }
    max_computed = 17;
}

/* compute a(n) for any n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not defined, but harmless */
    ensure_cache(n);
    init_terms();

    if (n <= max_computed) return cache[n];

    /* extend the cache up to n using the recurrence */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t pos_sum = (seq_t)0;
        seq_t neg_sum = (seq_t)0;

        for (idx_t i = 1; i <= 17; ++i) {
            seq_t v = cache[k - i];
            int c = coeff[i - 1];
            if (c >= 0)
                pos_sum += (seq_t)c * v;
            else
                neg_sum += (seq_t)(-c) * v;
        }
        cache[k] = pos_sum - neg_sum;      /* unsigned subtraction is safe */
    }
    max_computed = n;
    return cache[n];
}
