/*
 * A118866  Start with 1 and repeatedly place the first digit at the end of
 *          the number and add 20.
 * https://oeis.org/A118866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = rotate_first_digit(a(n-1)) + 20 with a(1) = 1
 * Range:     u8 n <= 12, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  rotate_first_digit(x):
    remove the most significant decimal digit d, move it to the end, and
    return the new number.  For x = d * 10^(k-1) + r   (0 <= r < 10^(k-1))
    the result is r * 10 + d.  */

#include <stdlib.h>
#include "../utils/digit_len.h"   /* int digit_len(seq_t x); */
#include "../utils/pow10.h"       /* seq_t pow10(idx_t e); */

static seq_t *cache = NULL;   /* cache of computed terms   */
static size_t cache_size = 0; /* number of terms stored   */
static size_t cache_cap  = 0; /* allocated capacity       */

static void
ensure_cache_cap(idx_t n)
{
    size_t need = (size_t)n;
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();   /* insufficient memory – terminate */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n<1 */

    /* Extend the cache up to index n */
    if (cache_size < (size_t)n) {
        ensure_cache_cap(n);
        /* Compute terms that are not yet cached */
        for (size_t i = cache_size; i < (size_t)n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;      /* a(1) */
                continue;
            }
            seq_t prev = cache[i - 1];    /* a(i)   */
            int d = digit_len(prev);     /* number of decimal digits (>=1) */
            seq_t pow10d1 = pow10((idx_t)(d - 1)); /* 10^(d-1) */
            seq_t first = prev / pow10d1;   /* most significant digit */
            seq_t rest  = prev % pow10d1;   /* remaining suffix   */
            seq_t rotated = rest * (seq_t)10 + first;
            cache[i] = rotated + (seq_t)20;
        }
        cache_size = (size_t)n;
    }

    return cache[n - 1];
}
