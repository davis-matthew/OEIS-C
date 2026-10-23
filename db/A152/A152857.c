/*
 * A152857  Period 5: repeat [0, 2, 3, 0, 0].
 * https://oeis.org/A152857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence: a(n+5)=a(n) with a(0)=0, a(1)=2, a(2)=3, a(3)=0, a(4)=0. */
seq_t a(idx_t n)
{
    switch (n % 5) {
        case 0: return 0;
        case 1: return 2;
        case 2: return 3;
        default: return 0;
    }
}
