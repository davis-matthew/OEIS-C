/*
 * A227192  Sum of the partial sums of the run lengths of binary expansion of
 *          n, when starting scanning from the least significant end; Row sums
 *          of A227188 and A227738.
 * https://oeis.org/A227192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = sum of partial sums of run lengths of the binary
 *            expansion of n, scanned from the least significant bit
 *            (rightmost)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at n = 1, but safe for n = 0 */
    idx_t x = n;                   /* make a copy so we don't modify the argument */
    seq_t total = 0;               /* cumulative value to return */
    seq_t cumulative = 0;          /* running sum of run lengths */

    while (x > 0) {
        uint64_t bit = x & 1;      /* current bit to count */
        uint64_t run = 0;          /* length of this run */

        /* Count how many consecutive bits equal 'bit' */
        while (x > 0 && ((x & 1) == bit)) {
            x >>= 1;
            ++run;
        }

        cumulative += (seq_t)run; /* shift the cumulative sum up by the run length */
        total += cumulative;      /* add this cumulative sum to the total */
    }

    return total;
}
