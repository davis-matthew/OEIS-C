/*
 * A044462  Numbers n such that string 0,4 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer N such that the base‑5 representation of
 *            * N contains the adjacent digit pair 0,4, while the
 *            representation of N+1 * does not. The sequence is indexed from 1
 *            (a(1)=29)
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Return true iff the base‑5 representation of x contains the substring
 * "0,4".  Scan from the least significant digit: a pair 0,4 means a
 * more‑significant 0 followed by a less‑significant 4, so in the reverse
 * scan we look for a digit 0 with a previous digit 4. */
static inline bool has04(seq_t x)
{
    seq_t prev = (seq_t)-1;          /* sentinel value > 4 */
    while (x) {
        seq_t d = x % 5;
        if (d == 0 && prev == 4)
            return true;
        prev = d;
        x /= 5;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;             /* only for compiler completeness */

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static idx_t cache_used = 0;
    static seq_t current = 1;        /* candidate to test */

    /* Grow cache up to the requested index */
    if (n > cache_used) {
        idx_t new_size = n;
        if (new_size < 16)
            new_size = 16;           /* initial allocation */

        /* Reallocate the cache array */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else if (new_size > cache_used) {
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }

        /* Find new terms and store them */
        for (; cache_used < n; ++cache_used) {
            /* Skip numbers that do not satisfy the definition */
            while (!(has04(current) && !has04(current + 1)))
                ++current;
            cache[cache_used] = current;
            ++current;                /* next candidate after the found term */
        }
    }

    return cache[(size_t)(n - 1)];
}
