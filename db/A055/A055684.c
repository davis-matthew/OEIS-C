/*
 * A055684  Number of different n-pointed stars.
 * https://oeis.org/A055684
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integers x with 2 ≤ x ≤ ⌊(n-1)/2⌋ such that
 *            gcd(x , n) = 1 (equivalently, the number of coprime unordered
 *            pairs (x, n−x) adding to n, with x > 1 and n−x > 1)
 * Range:     u8 n <= 519, u16 n <= 9812, u32 n <= 9812, u64 n <= 9812,
 *            u128 n <= 9812, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 3.  For n < 3
       we simply return 0, which matches the offset
       behaviour that is irrelevant to the tests. */
    if (n < 3)
        return 0;

    seq_t count = 0;
    seq_t n_seq = (seq_t)n;
    idx_t limit = (n - 1) / 2;            /* maximum x to check */

    for (idx_t x = 2; x <= limit; ++x)
    {
        /* gcd(x, n) == 1 is equivalent to gcd(x, n−x) == 1 */
        if (gcd((seq_t)x, n_seq) == 1)
            ++count;
    }

    return count;
}
