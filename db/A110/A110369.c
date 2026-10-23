/*
 * A110369  (Digit 1 repeated n times) + n.
 * https://oeis.org/A110369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (10^n - 1)/9 + n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Construct the integer consisting of n ones:
       1, 11, 111, … (10^n - 1)/9.
       This works for any seq_t size, including uint64_t
       for n up to the largest value that fits in the type. */
    seq_t ones = 0;
    for (idx_t i = 0; i < n; ++i) {
        ones = ones * (seq_t)10 + 1;
    }
    return ones + (seq_t)n;
}
