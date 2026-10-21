/*
 * A089596  G.f.:
 *          (1+2*x^3+3*x^6+x^8+6*x^9+2*x^11+9*x^12+x^14+10*x^15+x^16+9*x^18+2*x^19
 *          +6*x^21 +x^22+3*x^24+2*x^27+x^30) / ((1-x^5)^2*(1-x^12)^2).
 * https://oeis.org/A089596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in the generating function * G(x)
 *            = (1+2*x^3+3*x^6+x^8+6*x^9+2*x^11+9*x^12+x^14+10*x^15+x^16+ *
 *            9*x^18+2*x^19+6*x^21+x^22+3*x^24+2*x^27+x^30) / *
 *            ((1-x^5)^2*(1-x^12)^2). * * The denominator expands to * 1 -
 *            2x^5 + x^10 - 2x^12 + 4x^17 - 2x^22 + x^24 - 2x^29 + x^34,...
 * Range:     u8 n <= 43, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Numerator coefficients for 0 ≤ n ≤ 30. */
static const seq_t N_COEF[31] = {
    1, 0, 0, 2, 0, 0, 3, 0, 1, 6,
    0, 2, 9, 0, 1,10, 1, 0, 9, 2,
    0, 6, 1, 0, 3, 0, 0, 2, 0, 0,
    1
};

static inline seq_t Ncoeff(idx_t n)
{
    return n < 31 ? N_COEF[n] : 0;
}

/* Memoised cache of a(n). */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of valid entries in cache */

/* Return a(n).  Computes any missing entries up to n. */
seq_t a(idx_t n)
{
    size_t needed = (size_t)n + 1;

    /* Resize the cache if necessary. */
    if (needed > cache_size) {
        size_t old_size = cache_size;
        seq_t *tmp = cache ? (seq_t *)realloc(cache, needed * sizeof(seq_t))
                           : (seq_t *)malloc(needed * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = needed;

        /* Compute new entries starting from old_size. */
        for (size_t i = old_size; i < needed; ++i) {
            seq_t val = Ncoeff((idx_t)i);

            if (i >= 5)  val += (seq_t)2 * cache[i - 5];
            if (i >= 10) val -=                   cache[i - 10];
            if (i >= 12) val += (seq_t)2 * cache[i - 12];
            if (i >= 17) val -= (seq_t)4 * cache[i - 17];
            if (i >= 22) val += (seq_t)2 * cache[i - 22];
            if (i >= 24) val -=                   cache[i - 24];
            if (i >= 29) val += (seq_t)2 * cache[i - 29];
            if (i >= 34) val -=                   cache[i - 34];

            cache[i] = val;
        }
    }

    return cache[(size_t)n];
}
