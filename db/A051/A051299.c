/*
 * A051299  a(n) = a(n-1) + rotate( a(n-1), 1 digit left), a(1) = 1.
 * https://oeis.org/A051299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1; for n>1, a(n)=a(n-1)+rotate(a(n-1),1 digit left)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the number of decimal digits of x */
static inline int digit_len(seq_t x)
{
    int len = 1;
    while (x >= 10) {
        x /= 10;
        ++len;
    }
    return len;
}

/* Return 10^e as seq_t */
static inline seq_t pow10_seq(idx_t e)
{
    seq_t r = 1;
    while (e > 0) {
        r *= (seq_t)10;
        --e;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static seq_t *cache = NULL;
    static size_t cache_len = 0; /* number of terms currently stored */
    static size_t cache_cap = 0; /* allocation capacity */

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;
        cache_len = 0;
    }

    if ((size_t)n > cache_len) {
        if ((size_t)n > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < (size_t)n) new_cap = (size_t)n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;
            cache = tmp;
            cache_cap = new_cap;
        }

        for (idx_t k = cache_len + 1; k <= n; ++k) {
            if (k == 1) {
                cache[0] = (seq_t)1;
            } else {
                seq_t prev   = cache[k - 2];      /* a(k-1) */
                int   d      = digit_len(prev);   /* number of decimal digits */
                seq_t pow10d = pow10_seq((idx_t)(d - 1));
                seq_t first  = prev / pow10d;
                seq_t rest   = prev % pow10d;
                seq_t rot    = rest * (seq_t)10 + first;
                cache[k - 1] = prev + rot;       /* a(k)  */
            }
        }
        cache_len = (size_t)n;
    }

    return cache[n - 1];
}
