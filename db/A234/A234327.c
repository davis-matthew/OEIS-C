/*
 * A234327  Number of (n+1) X (2+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) +59*a(n-2) -768*a(n-3) +608*a(n-4) *
 *            +7580*a(n-5) -11628*a(n-6) -18088*a(n-7) +32880*a(n-8) *
 *            +3232*a(n-9) -17600*a(n-10) +4224*a(n-11) * Initial values:
 *            a(1)=432, a(2)=3296, a(3)=25728, a(4)=203008, * a(5)=1603680,
 *            a(6)=12713312, a(7)=100679584, a(8)=799588512, *
 *            a(9)=6341424800...
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t base_values[11] = {
    (seq_t)432ULL,
    (seq_t)3296ULL,
    (seq_t)25728ULL,
    (seq_t)203008ULL,
    (seq_t)1603680ULL,
    (seq_t)12713312ULL,
    (seq_t)100679584ULL,
    (seq_t)799588512ULL,
    (seq_t)6341424800ULL,
    (seq_t)50436411040ULL,
    (seq_t)400533940512ULL
};

static const int coeffs[11] = {
    10,  59, -768,  608,  7580, -11628,
   -18088, 32880, 3232, -17600, 4224
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* offset is 1, a(0) unused */

    static seq_t *cache = NULL;                  /* cache of previously computed terms */
    static size_t cache_size = 0;                /* number of terms stored (a(1) at index 0) */
    static size_t cache_cap  = 0;                /* allocated capacity */

    /* Ensure cache has space for up to term n */
    if (n > cache_cap) {
        size_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failed; abort computation */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    while (cache_size < (size_t)n) {
        seq_t val;
        if (cache_size < 11) {
            val = base_values[cache_size];
        } else {
            seq_t sum = 0;
            for (int i = 0; i < 11; ++i) {
                seq_t prev = cache[cache_size - 1 - i];
                int c = coeffs[i];
                if (c >= 0) {
                    sum += prev * (seq_t)c;
                } else {
                    sum -= prev * (seq_t)(-c);
                }
            }
            val = sum;
        }
        cache[cache_size] = val;
        ++cache_size;
    }

    return cache[n - 1];
}
