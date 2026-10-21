/*
 * A207102  Number of 0..n arrays x(0..4) of 5 elements with each no smaller
 *          than the sum of its two previous neighbors modulo (n+1).
 * https://oeis.org/A207102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -3*a(n-1) -2*a(n-2) +5*a(n-3) +12*a(n-4) +9*a(n-5)
 *            -3*a(n-6) -14*a(n-7) -19*a(n-8) -17*a(n-9) -3*a(n-10)
 *            +20*a(n-11) +32*a(n-12) +20*a(n-13) -3*a(n-14) -17*a(n-15)
 *            -19*a(n-16) -14*a(n-17) -3*a(n-18) +9*a(n-19) +12*a(n-20)
 *            +5*a(n-21) -2*a(n-22) -3*a(n-23) -a(n-24)
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 145, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init_vals[25] = {
    0,          /* unused index 0 */
    12, 55, 176, 441, 989, 1904, 3504, 5925, 9652, 14850,
    22390, 32305, 45920, 63323, 86112, 114393, 150567, 194218,
    248704, 313495, 392480, 485024, 596300, 724705
};

static const int coeff[24] = {
    -3, -2, 5, 12, 9, -3, -14, -19, -17, -3,
    20, 32, 20, -3, -17, -19, -14, -3, 9, 12,
    5, -2, -3, -1
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1 */

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;         /* highest index stored */

    /* initialize cache with the first 24 terms */
    if (cache == NULL) {
        size_t init_len = 24;
        cache = (seq_t*)calloc(init_len + 2, sizeof(seq_t));
        if (cache == NULL) return 0;    /* allocation failure */
        for (idx_t i = 1; i <= init_len; ++i)
            cache[i] = init_vals[i];
        cache_len = init_len;
    }

    /* extend cache if necessary */
    if (n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, (new_len + 2) * sizeof(seq_t));
        if (tmp == NULL) return 0;      /* allocation failure */
        cache = tmp;
        /* compute missing terms using the recurrence */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            __int128 sum = 0;
            for (int k = 0; k < 24; ++k) {
                /* i-k-1 is always > 0 because i > 24 when this loop runs */
                sum += (__int128)coeff[k] * (__int128)cache[i - k - 1];
            }
            cache[i] = (seq_t)sum;       /* truncation to seq_t width */
        }
        cache_len = new_len;
    }

    return cache[n];
}
