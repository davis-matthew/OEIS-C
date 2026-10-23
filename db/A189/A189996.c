/*
 * A189996  Bott periodicity: the homotopy groups of the stable orthogonal
 *          group are periodic with period 8 and repeat like [2, 2, 1, 0, 1,
 *          1, 1, 0].
 * https://oeis.org/A189996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = {2,2,1,0,1,1,1,0}[n mod 8]
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n & 7;          /* n mod 8 */
    switch (r) {
        case 0: return (seq_t)2;
        case 1: return (seq_t)2;
        case 2: return (seq_t)1;
        case 3: return (seq_t)0;
        case 4: return (seq_t)1;
        case 5: return (seq_t)1;
        case 6: return (seq_t)1;
        case 7: return (seq_t)0;
    }
    return (seq_t)0;           /* unreachable */
}
