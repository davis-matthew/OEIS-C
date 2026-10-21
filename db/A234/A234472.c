/*
 * A234472  Numbers that when raised to the fourth power and written backwards
 *          give squares.
 * https://oeis.org/A234472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 0.
 * For n > 1, a(n) is the (n‑2)-th number (0‑based) among all decimal numbers
 * that contain only the digits 0 and 1 and have at most two 1’s, when sorted
 * in increasing order and preceded by 0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Special case for the first term */
    if (n == 1u)
        return 0;

    /* m = number of terms after the initial 0 */
    idx_t m = n - 1;

    /* Find the minimal L such that L*(L+1)/2 >= m.
       L is the digit length of the desired number. */
    idx_t L = 0;
    while ((L * (L + 1)) / 2 < m)
        ++L;

    /* number of terms in all shorter lengths */
    idx_t prev = (L - 1) * L / 2;

    /* Position (1‑based) within the block of length L */
    idx_t offset = m - prev;          /* 1 <= offset <= L */

    /* Base number: 1 followed by (L‑1) zeros */
    seq_t value = pow10(L - 1);

    /* If we need a second 1, add the appropriate power of 10 */
    if (offset > 1)
        value += pow10(offset - 2);

    return value;
}
