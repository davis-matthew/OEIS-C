/*
 * A018842  Number of squares on infinite chessboard at n knight's moves from
 *          center.
 * https://oeis.org/A018842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, walk, easy, changed
 * Formula:   a(n) = 28*n-20 for n >= 5; a(0)=1, a(1)=8, a(2)=32, a(3)=68,
 *            a(4)=96
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) {
        switch ((unsigned long long)n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)8;
            case 2: return (seq_t)32;
            case 3: return (seq_t)68;
            case 4: return (seq_t)96;
            default: return (seq_t)0; /* unreachable, keeps compiler happy */
        }
    }
    return (seq_t)28 * (seq_t)n - (seq_t)20;
}
