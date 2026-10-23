/*
 * A121265  Descending dungeons: a(10)=10; for n>10, a(n) = a(n-1) read as if
 *          it were written in base n.
 * https://oeis.org/A121265
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(10) = 10; for n > 10, a(n) = a(n-1) read as if it were written
 *            in base n
 * Range:     u8 n <= 18, u16 n <= 21, u32 n <= 23, u64 n <= 26, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 10)
        return (seq_t)0;           /* below the offset – behaviour irrelevant */

    /* -------- static cache -------- */
    static seq_t *cache      = NULL;   /* array[0 .. cache_cap-1] */
    static idx_t  cache_cap  = 0;      /* allocated size */
    static idx_t  cache_max  = 0;      /* largest n that has been computed */

    /* enlarge cache to hold index n */
    if (cache_cap <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;           /* allocation failure – give up */
        /* initialize newly allocated cells to 0 */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* initialise first term a(10) if needed */
    if (cache[10] == (seq_t)0) {
        cache[10] = (seq_t)10;
        cache_max = (cache_max < 10) ? 10 : cache_max;
    }

    /* compute missing terms up to n */
    if (n > cache_max) {
        /* buffer for decimal digits of a(n-1), least‑significant first */
        static uint8_t *digits = NULL;
        static size_t   digits_cap = 0;

        seq_t base10 = (seq_t)10;

        for (idx_t cur = cache_max + 1; cur <= n; ++cur) {
            seq_t prev = cache[cur - 1];
            size_t pos = 0;

            if (prev == (seq_t)0) {    /* a special case for zero */
                if (digits_cap < 1) {
                    uint8_t *t = (uint8_t *)realloc(digits, 1);
                    if (!t) return (seq_t)0;
                    digits = t;
                    digits_cap = 1;
                }
                digits[0] = 0;
                pos = 1;
            } else {
                /* extract decimal digits into digits[] */
                while (prev > (seq_t)0) {
                    if (pos == digits_cap) {   /* need more space */
                        size_t new_cap = digits_cap ? digits_cap * 2 : 8;
                        uint8_t *t = (uint8_t *)realloc(digits, new_cap);
                        if (!t) return (seq_t)0;
                        digits = t;
                        digits_cap = new_cap;
                    }
                    digits[pos++] = (uint8_t)(prev % base10);
                    prev = prev / base10;
                }
            }

            /* build the new value interpreting digits in base 'cur' */
            seq_t result = (seq_t)0;
            for (size_t i = pos; i > 0; --i) {
                result = result * (seq_t)cur + (seq_t)digits[i - 1];
            }
            cache[cur] = result;
        }
        cache_max = n;
    }

    return cache[n];
}
