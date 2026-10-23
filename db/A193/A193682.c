/*
 * A193682  Period 8: repeat [0, 1, 2, 3, 0, 3, 2, 1].
 * https://oeis.org/A193682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = period 8: 0, 1, 2, 3, 0, 3, 2, 1
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n & 7;          /* r = n mod 8 (since 8 is 2³) */
    switch (r) {
        case 0: return (seq_t)0;
        case 1: return (seq_t)1;
        case 2: return (seq_t)2;
        case 3: return (seq_t)3;
        case 4: return (seq_t)0;
        case 5: return (seq_t)3;
        case 6: return (seq_t)2;
        case 7: return (seq_t)1;
        default: return (seq_t)0;   /* unreachable */
    }
}
