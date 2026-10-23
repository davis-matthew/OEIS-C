/*
 * A199570  Table, each row contains the previous sequence in odd columns and
 *          the row number in even columns.
 * https://oeis.org/A199570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) is produced from a triangular table where each row r *
 *            interleaves the previous sequence with the row number r. * For
 *            n>=1 let r = smallest integer with 3^(r-1) >= n. * Let m = n -
 *            3^(r-2). (For r=1, m=0.) * If m is even, a(n) = r; otherwise
 *            a(n) = a((m+1)/2)
 * Range:     u8 n <= 6561, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* Undefined for the OEIS offset (1), but safe. */
        return 0;
    if (n == 1)
        return 1;

    while (n > 1) {
        /* Find minimal e such that 3^e >= n */
        unsigned __int128 power = 1;
        unsigned int e = 0;          /* exponent e where power = 3^e */
        while ((unsigned __int128)n > power) {
            power *= 3;
            e++;
        }

        /* Row number r = e + 1 */
        idx_t r = (idx_t)(e + 1);

        /* Length of previous sequence = 3^(e-1) = power/3 */
        idx_t prev_len = (idx_t)(power / 3);

        idx_t m = n - prev_len;
        if ((m & 1U) == 0)            /* even -> return current row number */
            return (seq_t)r;

        /* odd -> recurse on the index (m+1)/2 */
        n = (m + 1) >> 1;
    }
    return 1;   /* base case for n == 1 */
}
