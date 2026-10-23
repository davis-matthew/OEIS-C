/*
 * A010169  Continued fraction for sqrt(98).
 * https://oeis.org/A010169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=9; for n>0, a(n)=1 if n is odd; if n is even, a(n)=8 when
 *            n%4==2 and a(n)=18 when n%4==0. This implements the 4‑periodic
 *            continued fraction terms for sqrt(98)
 * Range:     u8 n <= 8360, u16 n <= 8360, u32 n <= 8360, u64 n <= 8360,
 *            u128 n <= 8360, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)9;
    }
    if (n & 1) {          /* n is odd */
        return (seq_t)1;
    }
    /* n is even */
    switch (n & 3) {      /* n % 4 */
        case 2:
            return (seq_t)8;
        case 0:
            return (seq_t)18;
        default:
            /* unreachable for unsigned n */
            return (seq_t)0;
    }
}
