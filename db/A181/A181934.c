/*
 * A181934  Number of cents in U.S. Federal coinage, from 1793.
 * https://oeis.org/A181934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of cents in U.S. Federal coinage, from 1793. The
 *            sequence consists of the distinct denominations (in cents) that
 *            have ever been minted by the U.S. Treasury, sorted in increasing
 *            order
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* list of all denominations in cents, sorted */
    static const seq_t values[] = {
        1, 2, 3, 5, 10, 20, 25, 50, 100, 250, 300, 400, 500, 1000, 2000
    };
    const size_t len = sizeof(values) / sizeof(values[0]);

    /* OEIS offset is 1, so a(1) is the first term in values[] */
    if (n == 0)          /* out‑of‑range on the low side; return 0 */
        return 0;
    if ((idx_t)(n - 1) < len)   /* in‑range */
        return values[n - 1];
    return 0;          /* out‑of‑range on the high side; return 0 */
}
