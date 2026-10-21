/*
 * A073767  Bateman polynomial values n!*Z_n(-1).
 * https://oeis.org/A073767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n!*Z_n(-1)
 *           a(0)=1, a(1)=3, a(2)=20
 * Recurrence:
 *   n(2n-3)a(n) = (2n-1)(3n²-2n-4)a(n-1)
 *                - (2n-3)(3n²-10n+4)(n-1)a(n-2)
 *                + (n-1)(2n-1)(n-2)³a(n-3)
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t last   = (idx_t)-1;
    static size_t cap   = 0;

    /* initialise cache */
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) return (seq_t)0; /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)20;
        last = 2;
    }

    /* if requested value is already cached */
    if (n <= last) return cache[n];

    /* enlarge cache to hold up to n */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t newcap = cap ? cap << 1 : 4;
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cap = newcap;
    }

    /* compute remaining entries */
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t an;
        /* Use different paths depending on bit width of seq_t.
         * For 64‑bit (or smaller) handle intermediate arithmetic with
         * unsigned __int128 to avoid overflow. For larger seq_t,
         * use the native type (big integer or 128‑bit). */
        if (sizeof(seq_t) <= 8) {
            /* 64‑bit case: use 128‑bit intermediates */
            __uint128_t kk = k;
            __uint128_t two_minus_three = (2*kk - 3);
            __uint128_t coeff_left = kk * two_minus_three;

            __uint128_t coeff_a1 = (2*kk - 1) *
                                   ( (__uint128_t)3*kk*kk - 2*kk - 4 );
            __uint128_t coeff_a2 = (2*kk - 3) *
                                   ( (__uint128_t)3*kk*kk - 10*kk + 4 ) *
                                   (kk - 1);
            __uint128_t coeff_a3 = (kk - 1) *
                                   (2*kk - 1) *
                                   (kk - 2) * (kk - 2) * (kk - 2);

            __uint128_t term1 = coeff_a1 * (__uint128_t)cache[k-1];
            __uint128_t term2 = coeff_a2 * (__uint128_t)cache[k-2];
            __uint128_t term3 = coeff_a3 * (__uint128_t)cache[k-3];

            __uint128_t numerator = term1 - term2 + term3;
            an = (seq_t)(numerator / coeff_left);
        } else {
            /* larger seq_t (128‑bit or arbitrary precision) */
            seq_t kk = (seq_t)k;
            seq_t two_minus_three = (seq_t)(2*k - 3);
            seq_t coeff_left = kk * two_minus_three;

            seq_t coeff_a1 = (seq_t)(2*k - 1) *
                             ( (seq_t)3*kk*kk - 2*kk - 4 );
            seq_t coeff_a2 = (seq_t)(2*k - 3) *
                             ( (seq_t)3*kk*kk - 10*kk + 4 ) *
                             (kk - 1);
            seq_t coeff_a3 = (kk - 1) *
                             (seq_t)(2*k - 1) *
                             (kk - 2)*(kk - 2)*(kk - 2);

            seq_t term1 = coeff_a1 * cache[k-1];
            seq_t term2 = coeff_a2 * cache[k-2];
            seq_t term3 = coeff_a3 * cache[k-3];

            seq_t numerator = term1 - term2 + term3;
            an = numerator / coeff_left;
        }
        cache[k] = an;
    }

    last = n;
    return cache[n];
}
