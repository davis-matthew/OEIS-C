/*
 * A167924  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^16 = I.
 * https://oeis.org/A167924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A167924: a(0)=1, a(1)=16, a(2)=240, a(3)=3600, a(4)=54000,
 * a(5)=810000, a(6)=12150000, a(7)=182250000, a(8)=2733750000,
 * a(9)=41006250000, a(10)=615093750000, a(11)=9226406250000,
 * a(12)=138396093750000, a(13)=2075941406250000, a(14)=31139121093750000,
 * a(15)=467086816406250000.
 * For n ≥ 16: a(n) = 14*Σ_{j=1}^{15} a(n-j) – 105*a(n-16) + (n==16?1:0)
 * (the extra +1 comes from the numerator of the generating function for n=16).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t max_calculated = 0;   /* highest index already computed */
    /* Initial values (a(0) … a(15)) */
    static const seq_t init_vals[16] = {
        (seq_t)1ULL,
        (seq_t)16ULL,
        (seq_t)240ULL,
        (seq_t)3600ULL,
        (seq_t)54000ULL,
        (seq_t)810000ULL,
        (seq_t)12150000ULL,
        (seq_t)182250000ULL,
        (seq_t)2733750000ULL,
        (seq_t)41006250000ULL,
        (seq_t)615093750000ULL,
        (seq_t)9226406250000ULL,
        (seq_t)138396093750000ULL,
        (seq_t)2075941406250000ULL,
        (seq_t)31139121093750000ULL,
        (seq_t)467086816406250000ULL
    };

    /* First call: allocate at least 16 entries */
    if (cache == NULL) {
        size_t sz = (size_t)(n > 15 ? n : 15) + 1;
        /* ensure minimum size for initial values */
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure */
        for (idx_t i = 0; i < 16; ++i) cache[i] = init_vals[i];
        max_calculated = 15;
    }

    /* If required index already computed */
    if (n <= max_calculated) return cache[n];

    /* Grow cache to hold up to n */
    if ((size_t)(n + 1) > (size_t)max_calculated + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
    }

    /* Compute from max_calculated+1 up to n */
    for (idx_t i = max_calculated + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        /* sum a(i-1) .. a(i-15) */
        for (idx_t j = 1; j <= 15; ++j)
            sum += cache[i - j];
        seq_t term1 = sum * (seq_t)14;
        seq_t term2 = cache[i - 16] * (seq_t)105;
        seq_t val = term1 - term2;
        if (i == 16) val += (seq_t)1;  /* numerator contribution for n = 16 */
        cache[i] = val;
    }
    max_calculated = n;
    return cache[n];
}
