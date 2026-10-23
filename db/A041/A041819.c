/*
 * A041819  Denominators of continued fraction convergents to sqrt(430).
 * https://oeis.org/A041819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 5724502*a(n-14) – a(n-28) for n ≥ 28 Initial values
 *            (convergents to √430): a(0)=1, a(1)=1, a(2)=3, a(3)=4, a(4)=15,
 *            a(5)=19, a(6)=129, a(7)=1051, a(8)=6435, a(9)=7486, a(10)=28893,
 *            a(11)=36379, a(12)=101651, a(13)=138030, a(14)=5622851,
 *            a(15)=5760881, a(16)=17144613, a(17)=22905494, a(18)=...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"

/* Compute the first 28 denominators of convergents to sqrt(430).  */
static void compute_initial_terms(seq_t out[28]) {
    const uint64_t D = 430;
    const uint64_t a0 = 20;          /* floor(sqrt(430)) */
    uint64_t m = 0, d = 1, a = a0;

    seq_t p_kkm1 = 0;  /* p[-2] */
    seq_t p_km1  = 1;  /* p[-1] */
    seq_t q_kkm1 = 1;  /* q[-2] */
    seq_t q_km1  = 0;  /* q[-1] */

    for (size_t k = 0; k < 28; ++k) {
        /* p_k = a * p_{k-1} + p_{k-2}   */
        seq_t p_k = (seq_t)a * p_km1 + p_kkm1;
        /* q_k = a * q_{k-1} + q_{k-2}   */
        seq_t q_k = (seq_t)a * q_km1 + q_kkm1;

        out[k] = q_k;

        /* shift previous values for next iteration */
        p_kkm1 = p_km1;  p_km1 = p_k;
        q_kkm1 = q_km1;  q_km1 = q_k;

        /* next partial quotient */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
    }
}

/* Singleton cache for sequence values */
static seq_t *cache    = NULL;
static size_t cache_size = 0;  /* number of elements stored */
static size_t cache_cap  = 0;  /* allocated capacity */

/* Public sequence function */
seq_t a(idx_t n) {
    /* Ensure cache is initialised and big enough for requested index */
    if (cache_size == 0) {
        /* Initialise first 28 terms */
        seq_t first28[28];
        compute_initial_terms(first28);

        /* Allocate cache of at least 28 elements or n+1 if larger */
        size_t needed = (n + 1 > 28) ? (size_t)(n) + 1 : 28;
        ensure_capacity(needed, &cache_cap, (void **)&cache, sizeof(seq_t));

        /* Copy initial terms */
        for (size_t i = 0; i < 28; ++i)
            cache[i] = first28[i];

        /* Compute further terms up to 'needed' */
        for (size_t i = 28; i < needed; ++i) {
            cache[i] = ((seq_t)5724502) * cache[i - 14] - cache[i - 28];
        }

        cache_size = needed;
    }

    /* Expand cache if requested n exceeds current size */
    if ((size_t)n + 1 > cache_size) {
        size_t new_needed = (size_t)n + 1;
        ensure_capacity(new_needed, &cache_cap, (void **)&cache, sizeof(seq_t));
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            cache[i] = ((seq_t)5724502) * cache[i - 14] - cache[i - 28];
        }
        cache_size = new_needed;
    }

    return cache[(size_t)n];
}
