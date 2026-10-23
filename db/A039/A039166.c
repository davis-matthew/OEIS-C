/*
 * A039166  Numbers whose base-11 representation has the same number of 1's
 *          and 3's.
 * https://oeis.org/A039166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base-11 representation has equally many 1's and
 *            3's
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for already computed values */
    static seq_t *cache = NULL;
    static size_t cached_size = 0;   /* number of values stored */
    static seq_t next_candidate = 0; /* next integer to test */

    if (n == 0) return 0;            /* undefined, but safe */

    /* Expand the cache until it contains at least n elements */
    if (cached_size < (size_t)n) {
        size_t new_size = (size_t)n;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        for (size_t i = cached_size; i < new_size; ++i) {
            seq_t x = next_candidate;     /* start with the next unused integer */

            /* Search for the next integer whose base‑11 digits contain
               the same number of 1's and 3's. */
            while (1) {
                idx_t count1 = 0;
                idx_t count3 = 0;
                seq_t t = x;

                /* Count 1's and 3's in the base‑11 representation */
                if (t != 0) {
                    while (t > 0) {
                        seq_t d = t % 11;
                        if (d == 1) ++count1;
                        else if (d == 3) ++count3;
                        t /= 11;
                    }
                }
                /* For zero, count1 and count3 remain 0, so the condition holds */

                if (count1 == count3) {
                    break;           /* found a valid term */
                }
                ++x;                  /* try next integer */
            }

            cache[i] = x;           /* store the found term */
            next_candidate = x + 1; /* next search starts after it */
        }

        cached_size = new_size;
    }

    return cache[n - 1];
}
