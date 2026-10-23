/*
 * A178703  Partial sums of round(3^n/7).
 * https://oeis.org/A178703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = floor((3*3^n - 1)/14) (OEIS A178703)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Conservative implementation using recurrence:
   a(0)=0,a(1)=0,a(2)=1,a(3)=5,a(4)=17,a(5)=52
   for n>5: a(n)=a(n-6)+52*3^(n-5)
   Cache values to support arbitrary call order and n up to ~10000. */

#include <stdlib.h>
#include <stdint.h>         /* for uint64_t */

static seq_t *cache   = NULL;   /* cached a(n) values             */
static idx_t cache_len = 0;     /* number of computed values      */
static size_t cache_cap = 0;    /* allocation capacity of cache   */

static seq_t *pow3_cache = NULL;   /* cached powers of 3: 3^i      */
static idx_t pow3_len = 0;         /* number of computed powers    */
static size_t pow3_cap = 0;        /* allocation capacity of pow3  */

static void ensure_cache_capacity(idx_t needed)
{
    while (cache_cap < (size_t)needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)needed)
            new_cap = (size_t)needed;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

static void ensure_pow3_capacity(idx_t needed)
{
    while (pow3_cap < (size_t)needed) {
        size_t new_cap = pow3_cap ? pow3_cap * 2 : 16;
        if (new_cap < (size_t)needed)
            new_cap = (size_t)needed;
        pow3_cache = (seq_t *)realloc(pow3_cache, new_cap * sizeof(seq_t));
        pow3_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n >= cache_len) {
        /* enlarge cache to include n */
        ensure_cache_capacity(n + 1);

        /* initialise first six values */
        static const seq_t init[6] = {0, 0, 1, 5, 17, 52};

        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 6) {
                cache[i] = init[i];
            } else {
                /* ensure we have powers up to 3^(i-5) */
                idx_t need_pow = i - 5;          /* exponent required */
                ensure_pow3_capacity(need_pow + 1);

                /* compute new powers of 3 if missing */
                while (pow3_len <= need_pow) {
                    if (pow3_len == 0) {
                        pow3_cache[0] = (seq_t)1;
                    } else {
                        pow3_cache[pow3_len] = pow3_cache[pow3_len - 1] * (seq_t)3;
                    }
                    ++pow3_len;
                }

                /* recurrence: a(i) = a(i-6) + 52 * 3^(i-5) */
                cache[i] = cache[i - 6] + (seq_t)52 * pow3_cache[i - 5];
            }
        }
        cache_len = n + 1;
    }
    return cache[n];
}
