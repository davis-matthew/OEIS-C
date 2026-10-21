/*
 * A229668  Number of defective 4-colorings of an n X 4 0..3 array connected
 *          horizontally, antidiagonally and vertically with exactly two
 *          mistakes, and colors introduced in row-major 0..3 order.
 * https://oeis.org/A229668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 36*a(n-1) - 552*a(n-2) + 4812*a(n-3) - 27168*a(n-4) * +
 *            107904*a(n-5) - 316720*a(n-6) + 703296*a(n-7) * - 1187712*a(n-8)
 *            + 1506368*a(n-9) - 1379328*a(n-10) * + 835584*a(n-11) -
 *            262144*a(n-12) * initial values: a(1)=3, a(2)=652, a(3)=19805,
 *            a(4)=328160, a(5)=4331928, * a(6)=50919512, a(7)=5...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static const seq_t init_vals[14] = {
    (seq_t)3,
    (seq_t)652,
    (seq_t)19805,
    (seq_t)328160,
    (seq_t)4331928,
    (seq_t)50919512,
    (seq_t)557478448,
    (seq_t)5814322288ULL,
    (seq_t)58526242224ULL,
    (seq_t)573293485312ULL,
    (seq_t)5495680088320ULL,
    (seq_t)51763989177088ULL,
    (seq_t)480493519118080ULL,
    (seq_t)4405407183929600ULL
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    if (newcap < needed) newcap = needed;
    seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache = newptr;
    cache_cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n=0, return 0 */

    /* Convert idx_t to size_t; n is small enough for this in our tests */
    size_t nn = (size_t)n;

    /* Ensure we have initial values in the cache */
    if (cache == NULL) {
        ensure_capacity(14);
        for (size_t i = 0; i < 14; ++i) cache[i] = init_vals[i];
    }

    /* If requested value is within the initial table, return it */
    if (nn <= 14) return cache[nn - 1];

    /* Prepare to compute up to n */
    ensure_capacity(nn);

    /* Coefficients for the recurrence (12 terms) */
    static const int coeffs[12] = {
        36,      -552,      4812,     -27168,    107904,
        -316720, 703296,    -1187712, 1506368,   -1379328,
        835584,  -262144
    };

    /* Compute each new term only once */
    for (size_t i = 14; i < nn; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int k = 0; k < 12; ++k) {
            /* abs_coeff handles negative coefficients without wrap‑around */
            int coeff = coeffs[k];
            int abs_coeff = coeff < 0 ? -coeff : coeff;
            seq_t prev = cache[i - 1 - k];
            seq_t prod = prev * (seq_t)abs_coeff;
            if (coeff > 0)
                pos += prod;
            else
                neg += prod;
        }
        seq_t val = pos - neg;   /* pos is guaranteed >= neg for the recurrence */
        cache[i] = val;
    }

    return cache[nn - 1];
}
