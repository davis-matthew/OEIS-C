/*
 * A011581  Stirling numbers of second kind S2(32,n).
 * https://oeis.org/A011581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = Stirling number of the second kind S(32, n)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* For n < 1 or n > 32 the entry is zero (outside the table). */
    if (n < 1 || n > 32u)
        return 0;

    /* Static cache for row 32: S(32, n) for n = 0..32 (index 0 unused). */
    static seq_t s[33];
    static bool initialized = false;

    if (!initialized) {
        /* Initialise all cells to zero. */
        for (size_t i = 0; i < 33; ++i)
            s[i] = 0;
        /* Base row : S(1,1) = 1. */
        s[1] = 1;

        /* Build successive rows k = 2 .. 32.  We update s in place
           iterating n from k down to 1 so that s[n] always contains
           the value from the previous row. */
        for (idx_t k = 2; k <= 32; ++k) {
            for (idx_t ncur = k; ncur >= 1; --ncur) {
                /* S(k, ncur) = ncur * S(k-1, ncur) + S(k-1, ncur-1) */
                s[(size_t)ncur] =
                    (seq_t)ncur * s[(size_t)ncur] + s[(size_t)(ncur - 1)];
            }
        }
        initialized = true;
    }

    return s[(size_t)n];
}
