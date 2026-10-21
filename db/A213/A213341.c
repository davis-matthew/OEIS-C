/*
 * A213341  Smallest palindrome with digital root n and n + 1 decimal digits.
 * https://oeis.org/A213341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = smallest palindrome with n+1 decimal digits and * sum of
 *            digits = 10*n (which gives digital root n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Length of the palindrome */
    idx_t L = n + 1;

    /* Allocate array for digits */
    size_t len = (size_t)L;
    uint8_t *digits = (uint8_t *)malloc(len);
    if (!digits)
        return 0; /* allocation failure; should not happen for tests */

    /* Target sum of digits: 10*n */
    uint64_t target = 10u * (uint64_t)n;
    uint64_t rem = target;

    size_t m = len / 2;          /* number of mirrored pairs */
    bool odd = (len % 2) != 0;   /* true if middle digit exists */

    size_t i;
    for (i = 0; i < m; ++i) {
        size_t pairs_left = m - i - 1;
        uint64_t max_rem = pairs_left * 18ULL + (odd ? 9ULL : 0ULL);

        uint8_t low = (i == 0) ? 1 : 0; /* leading digit must be ≥1 */

        uint8_t d;
        for (d = low; d <= 9; ++d) {
            uint64_t sub = 2ULL * (uint64_t)d;
            if (rem >= sub && rem - sub <= max_rem) {
                digits[i] = d;
                digits[len - 1 - i] = d;
                rem -= sub;
                break;
            }
        }
        /* For valid n the loop always finds a digit. */
    }

    if (odd) {
        /* Remaining sum must fit into the middle digit. */
        digits[len / 2] = (uint8_t)rem;
    }

    /* Assemble the integer value. */
    seq_t result = 0;
    for (i = 0; i < len; ++i) {
        result = result * (seq_t)10 + digits[i];
    }

    free(digits);
    return result;
}
