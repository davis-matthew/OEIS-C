/*
 * A129837  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+119)^2 = y^2.
 * https://oeis.org/A129837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-9) - a(n-18) + 238 for n > 18 Initial values:
 *            a(1)=0, a(2)=24, a(3)=49, a(4)=57, a(5)=85, a(6)=136, a(7)=180,
 *            a(8)=196, a(9)=261, a(10)=357, a(11)=481, a(12)=616, a(13)=660,
 *            a(14)=816, a(15)=1105, a(16)=1357, a(17)=1449, a(18)=1824
 * Range:     u8 n <= 8, u16 n <= 36, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;           /* cache[i] holds a(i+1)          */
static size_t cache_len = 0;          /* number of terms stored         */
static size_t cache_cap = 0;          /* allocated capacity of cache    */

static const seq_t init[18] = {
    0,    24,   49,   57,   85,   136,   180,   196,   261,
    357,  481,  616,  660,  816, 1105, 1357, 1449, 1824
};

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; guard against an accidental
       0 index.                                          */
    if (n == 0) return 0;

    /* Ensure we have enough space in the cache.  Use a
       reallocation that works in both C and C++: cast the result
       to seq_t*.  For builds where seq_t is a trivial type this is
       fine.  For big‑integer builds the harness guarantees that the
       type can be allocated with memcpy.                      */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 64 : cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;   /* grow by powers of two */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                           /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n.          */
    if ((size_t)n > cache_len) {
        for (size_t idx = cache_len + 1; idx <= (size_t)n; ++idx) {
            seq_t val;
            if (idx <= 18) {
                val = init[idx - 1];
            } else {
                /* Recurrence: a(n) = 6*a(n-9) - a(n-18) + 238              */
                val = 6 * cache[(idx - 9) - 1]      /* a(idx-9) */
                    - cache[(idx - 18) - 1]         /* a(idx-18) */
                    + (seq_t)238;
            }
            cache[idx - 1] = val;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
