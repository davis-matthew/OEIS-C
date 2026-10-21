/*
 * A211663  Number of iterations log(log(log(...(n)...))) such that the result
 *          is < 1.
 * https://oeis.org/A211663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 1 for n < 3, * 2 for 3 ≤ n < 16, * 3 otherwise * (number
 *            of iterations of log needed to get below 1)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 3)               return 1;
    if (n <= 15)             return 2;
    return 3;
}
