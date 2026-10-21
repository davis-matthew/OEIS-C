/*
 * A071377  Number of positive integers <= n which are the sum of 3 squares.
 * https://oeis.org/A071377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + 1) - #numbers ≤ n of the form 4^a*(8b+7) * – [ (n +
 *            1) is of the form 4^a*(8b+7) ] * This implements the Legendre
 *            three‑square theorem count
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   is_bad_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_bad_number.h"

seq_t a(idx_t n)
{
    /* Count how many integers ≤ n are of the forbidden form. */
    idx_t div = 1;          /* current power of 4 (4^a) */
    seq_t bad = 0;          /* number of forbidden numbers ≤ n */

    while (div <= n) {
        idx_t m0 = n / div;           /* largest multiple of div not exceeding n */
        if (m0 >= 7) {
            /* Count m such that div*(8m+7) ≤ n:
             * 0 ≤ m ≤ floor((m0 - 7)/8)
             * Count = floor((m0-7)/8) + 1
             */
            bad += (seq_t)((m0 - 7U) / 8U + 1U);
        }
        if (div > n / 4U)           /* next power would overflow n */
            break;
        div *= 4U;
    }

    /* Number of non‑forbidden integers up to n (including 0). */
    seq_t total = (seq_t)n + 1U;
    seq_t result = total - bad;

    /* Subtract one if the next integer n+1 itself is forbidden,
     * because the sequence a(n) does not count it even though it
     * does not appear in the range [0,n]. */
    idx_t next = n + 1U;
    if (is_bad_number(next))
        result -= 1U;

    return result;
}
