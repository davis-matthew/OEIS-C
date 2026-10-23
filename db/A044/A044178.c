/*
 * A044178  Numbers n such that string 5,5 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044178: numbers n such that
   the base‑7 representation of n contains the substring “55”
   but the representation of n‑1 does not.  
   The implementation scans integers from 1 upwards, storing all
   found terms in a static vector so that subsequent calls in any
   order remain efficient. */

#include <stdlib.h>
#include <stdbool.h>

/* Helper: test whether a number has adjacent 5‑digits in base 7 */
static bool has55(seq_t x)
{
    seq_t prev = 7;               /* sentinel not equal to 5 */
    while (x > 0) {
        seq_t digit = x % 7;
        if (digit == 5 && prev == 5)
            return true;
        prev = digit;
        x /= 7;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* stored terms */
    static idx_t cache_size = 0;        /* number of terms stored */
    static idx_t cache_cap = 0;         /* capacity of cache array */
    static seq_t next_start = 1;        /* next integer to test */

    /* Generate terms until we have at least n of them */
    while (cache_size < n) {
        seq_t cur = next_start;
        /* find next candidate satisfying the property */
        while (true) {
            if (has55(cur)) {
                if (!has55(cur - 1)) {
                    /* store */
                    if (cache_size == cache_cap) {
                        size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                        if (!tmp)
                            abort();         /* memory allocation failed */
                        cache = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_size++] = cur;
                    next_start = cur + 1;
                    break;
                }
            }
            cur++;
        }
    }
    return cache[n - 1];
}
