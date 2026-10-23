/*
 * A130659  Period 4: repeat [0, 1, 2, 4].
 * https://oeis.org/A130659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) with period 4: [0,1,2,4] Initial values: a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=4
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n & 3) {
        case 0: return 0;
        case 1: return 1;
        case 2: return 2;
        default: return 4; /* case 3 */
    }
}
