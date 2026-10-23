/*
 * A044330  Numbers n such that string 8,7 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 889, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth integer whose base‑9 representation contains the
 * digit pair 8 followed by 7, but whose predecessor n‑1 does not. */

/* The implementation caches the sequence so that calls in any
 * order are fast. It works for both plain unsigned integer
 * seq_t types (uint8_t … uint64_t, unsigned __int128) and for
 * arbitrary‑precision seq_t classes. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return true iff the base‑9 representation of x contains
 * the consecutive digits 8,7. */
static bool has_pattern(idx_t x)
{
    if (x == 0)
        return false;

    unsigned char digits[64];
    int len = 0;
    while (x > 0) {
        digits[len++] = (unsigned char)(x % 9);
        x = x / 9;
    }
    for (int i = len - 1; i > 0; --i) {
        if (digits[i] == 8 && digits[i - 1] == 7)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t count = 0;        /* number of terms stored */
    static idx_t last_checked = 0;  /* largest integer examined so far */

    while (count < n) { /* generate until we have at least n terms */
        last_checked++;

        bool now  = has_pattern(last_checked);
        bool prev = has_pattern(last_checked - 1);

        if (now && !prev) {
            if (count == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_ptr) abort();          /* memory exhaustion */
                cache = new_ptr;
                cache_cap = new_cap;
            }
            cache[count] = (seq_t)last_checked;  /* promotion to seq_t */
            ++count;
        }
    }
    return cache[n - 1];
}
