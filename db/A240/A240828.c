/*
 * A240828  a(1)=a(2)=0, a(3)=2; thereafter a(n) = Sum( a(n-i-s-a(n-i-1)),
 *          i=0..k-1 ), where s=0, k=3.
 * https://oeis.org/A240828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, hear, easy
 * Formula:   a(2h) = 2*floor(h/2), a(2h+1) = 2h (h≥0)
 * Range:     u8 n <= 255, u16 n <= 9629, u32 n <= 9629, u64 n <= 9629,
 *            u128 n <= 9629, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;
    if (n_s <= 2) return 0;
    if (n_s & 1) return n_s - 1;           /* odd: a(2h+1) = 2h */
    seq_t h = n_s / 2;                     /* h = n/2   for even n */
    return (h / 2) * 2;                    /* 2*floor(h/2)  */
}
