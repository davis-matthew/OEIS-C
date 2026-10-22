/*
 * A007798  Expected number of random moves in Tower of Hanoi problem with n
 *          disks starting with a randomly chosen position and ending at a
 *          position with all disks on the same peg.
 * https://oeis.org/A007798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 15*a(n-2) + 2, a(0) = 0, a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prev2 = 0, prev1 = 0, cur;
    idx_t i;
    if (n < 2)
        return 0;
    for (i = 2; i <= n; i++) {
        /* only + - * : exact modulo 2^bits, so correct whenever a(n) fits */
        cur = (seq_t)8 * prev1 - (seq_t)15 * prev2 + 2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
