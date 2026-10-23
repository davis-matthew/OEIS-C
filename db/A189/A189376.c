/*
 * A189376  Expansion of 1/((1-x)^5*(x^3+x^2+x+1)^2).
 * https://oeis.org/A189376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 65, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + 2*a(n-4) - 6*a(n-5)
          + 6*a(n-6) - 2*a(n-7) - a(n-8) + 3*a(n-9) - 3*a(n-10) + a(n-11)
   Initial values: a(0)=1, a(1)=3, a(2)=6, a(3)=10, a(4)=17, a(5)=27,
                   a(6)=40, a(7)=56, a(8)=78, a(9)=106, a(10)=140.
*/
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

static const seq_t init_vals[11] = {
    1, 3, 6, 10, 17, 27, 40, 56, 78, 106, 140
};

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    if (idx <= 10) {
        return init_vals[idx];
    }

    /* Ensure cache is initialized with the first 11 terms. */
    if (!cache) {
        cache_len = 11;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;       /* allocation failed */
        for (size_t i = 0; i < 11; ++i) {
            cache[i] = init_vals[i];
        }
    }

    if (idx >= cache_len) {
        size_t new_len = idx + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        for (size_t i = cache_len; i <= idx; ++i) {
            seq_t val = ((seq_t)3)*cache[i-1]
                       - ((seq_t)3)*cache[i-2]
                       + cache[i-3]
                       + ((seq_t)2)*cache[i-4]
                       - ((seq_t)6)*cache[i-5]
                       + ((seq_t)6)*cache[i-6]
                       - ((seq_t)2)*cache[i-7]
                       - cache[i-8]
                       + ((seq_t)3)*cache[i-9]
                       - ((seq_t)3)*cache[i-10]
                       + cache[i-11];
            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[idx];
}
