/*
 * A039581  Numbers whose base-6 representation has the same number of 0's,
 *          3's and 5's.
 * https://oeis.org/A039581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039581: numbers whose base‑6 representation has equal
   numbers of digits 0, 3, and 5. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Count the digits 0, 3 and 5 in the base‑6 representation of x.
   Return true only if all three counts are equal. */
static bool good_number(seq_t x)
{
    unsigned cnt0 = 0, cnt3 = 0, cnt5 = 0;
    seq_t y = x;
    while (y != 0) {
        unsigned d = (unsigned)(y % 6);
        if (d == 0)       ++cnt0;
        else if (d == 3)  ++cnt3;
        else if (d == 5)  ++cnt5;
        y /= 6;
    }
    return (cnt0 == cnt3 && cnt3 == cnt5);
}

/* Return the n‑th term of the sequence (a(1) = 1). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of found terms */
    static size_t cache_len = 0;     /* number of stored terms */
    static size_t cache_cap = 0;     /* allocated capacity */
    static seq_t next_test = 1;      /* next integer to test */

    if (n == 0) return (seq_t)0;     /* undefined for n < 1, but harmless */

    if (n > cache_len) {
        /* Ensure the cache can hold at least n terms. */
        if (cache_cap < n) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            if (new_cap < n) new_cap = n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failure -> fallback */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Generate terms until the cache contains n terms. */
        while (cache_len < (size_t)n) {
            if (good_number(next_test)) {
                cache[cache_len++] = next_test;
            }
            ++next_test;
        }
    }

    return cache[n - 1];
}
