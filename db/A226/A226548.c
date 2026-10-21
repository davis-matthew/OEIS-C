/*
 * A226548  Number of squares in all tilings of a 5 X n rectangle using
 *          integer-sided square tiles.
 * https://oeis.org/A226548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the order‑16 linear recurrence derived from the
 *            generating function G.f. = (x^10 +12*x^9 +28*x^8 +54*x^7 +33*x^6
 *            +18*x^5 -53*x^4 -42*x^3 +6*x^2 +30*x+5)*x / (x^8 +3*x^7 +2*x^6
 *            +5*x^5 +x^4 -6*x^3 -7*x^2 -2*x+1)^2. The denominator squared
 *            gives the recurrence coefficients d[1]..d[16]...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static const int d[17] = {
    0,           /* unused index 0 */
   -4, -10,  16,  75,  90,   6, -84, -97, -60, -21,
   14,  36,  22,  13,   6,   1
};

static const int rarr[12] = {
     0,   5,  30,   6, -42, -53, 18, 33,
    54,  28,  12,   1
};

static seq_t *cache     = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_cap >= needed) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;

#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
    delete [] cache;
    cache = tmp;
#else
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
#endif
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);

    for (size_t k = cache_len; k <= (size_t)n; ++k) {
        seq_t sum = 0;

        if (k <= 11) {
            int r = rarr[k];
            if (r >= 0) {
                sum += (seq_t)r;
            } else {
                sum -= (seq_t)(-r);
            }
        }

        for (int i = 1; i <= 16; ++i) {
            if (k < (size_t)i) continue;
            const seq_t term = cache[k - i];
            int coeff = d[i];
            if (coeff >= 0) {
                sum -= (seq_t)coeff * term;
            } else {
                sum += (seq_t)(-coeff) * term;
            }
        }

        cache[k] = sum;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
