/*
 * A005994  Alkane (or paraffin) numbers l(7,n).
 * https://oeis.org/A005994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = [(n+1)*(n+3)*((n+2)*(n+4)+3) + (3*(2*n+5) if n even, 0 if
 *            n odd)] / 48
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 276, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n1 = (seq_t)(n + 1);
    seq_t n2 = (seq_t)(n + 2);
    seq_t n3 = (seq_t)(n + 3);
    seq_t n4 = (seq_t)(n + 4);
    
    seq_t inner = n2 * n4 + 3;
    seq_t numerator = n1 * n3 * inner;
    
    if (n % 2 == 0) {
        numerator += 3 * (2 * (seq_t)n + 5);
    }
    
    return numerator / 48;
}
