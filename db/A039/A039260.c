/*
 * A039260  Numbers whose base-12 representation has the same number of 6's
 *          and 7's.
 * https://oeis.org/A039260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration of integers whose base‑12 representation contains an equal
   number of 6's and 7's. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Persistent cache for memoisation */
    static seq_t *cache = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static idx_t next = 0;          /* next candidate to test */

    /* n is 1‑based; for safety we handle n==0 */
    if (n == 0) return 0;

    while (len < (size_t)n) {
        idx_t val = next++;        /* candidate number */

        /* Count 6's and 7's in base‑12 */
        int c6 = 0, c7 = 0;
        if (val != 0) {
            idx_t t = val;
            while (t > 0) {
                int d = t % 12;
                if (d == 6) ++c6;
                else if (d == 7) ++c7;
                t /= 12;
            }
        }
        if (c6 == c7) {            /* property satisfied */
            if (cap == 0) {
                cap = 128;
                cache = (seq_t *)malloc(cap * sizeof(seq_t));
            } else if (len == cap) {
                cap <<= 1;          /* double the capacity */
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = (seq_t)val;
        }
    }

    return cache[(size_t)n - 1];
}
