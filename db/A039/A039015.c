/*
 * A039015  Numbers whose base-5 representation has the same number of 3's and
 *          4's.
 * https://oeis.org/A039015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th non‑negative integer whose base‑5
 *            representation contains an equal number of digits 3 and 4. This
 *            implementation uses an incremental search; the result is
 *            memoised so that subsequent calls are fast
 * Range:     u8 n <= 94, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cached sequence values     */
    static size_t cap   = 0;             /* current capacity of cache  */
    static size_t sz    = 0;             /* number of values stored    */
    static seq_t last_checked = 0;       /* next candidate to test     */

    /* Ensure enough memory for the cache.  Cap grows geometrically. */
    if (sz >= n)
        return cache[(size_t)n - 1];

    /* Expand cache as needed.  Start with capacity 4. */
    if (cap == 0) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Search for further qualifying numbers until we have n values. */
    while (sz < n) {
        seq_t candidate = last_checked++;
        int c3 = 0, c4 = 0;
        seq_t tmp = candidate;

        /* Count 3's and 4's in base‑5 representation. */
        if (tmp == 0) {
            /* Zero has no 3's or 4's, so it qualifies. */
        } else {
            while (tmp > 0) {
                int digit = (int)(tmp % 5);
                if (digit == 3) ++c3;
                else if (digit == 4) ++c4;
                tmp /= 5;
            }
        }

        if (c3 == c4) {
            /* Store the qualifying number. */
            if (sz == cap) {
                cap <<= 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[sz++] = candidate;
        }
    }

    return cache[(size_t)n - 1];
}
