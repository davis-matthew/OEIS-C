/*
 * A087117  Number of zeros in the longest string of consecutive zeros in the
 *          binary representation of n.
 * https://oeis.org/A087117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = longest run of consecutive zeros *
 *            in the binary representation of n. * This follows directly from
 *            the OEIS definition: * a(n) = number of zeros in the longest
 *            string of consecutive zeros * in the binary representation of n.
 *            For n = 0 the binary string * is "0", hence...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the longest consecutive run of zero bits in n.
 * Since n ≤ 2^k-1, the longest run length cannot exceed the bit
 * width of n (≤ 64 for idx_t).  Thus the result always fits in seq_t. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Binary representation of 0 is "0", so the longest zero run
           has length 1. */
        return (seq_t)1;
    }

    seq_t max_zeros = 0;
    seq_t cur_zeros = 0;
    // Scan bits from LSB to MSB
    while (n) {
        if ((n & (idx_t)1) == 0) {      // current bit is 0
            cur_zeros++;
            if (cur_zeros > max_zeros)
                max_zeros = cur_zeros;
        } else {                          // current bit is 1
            cur_zeros = 0;
        }
        n >>= 1;
    }
    return max_zeros;
}
