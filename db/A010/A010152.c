/*
 * A010152  Continued fraction for sqrt(74).
 * https://oeis.org/A010152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=8; for n>=1, a(n)=16 if n%5==0 else 1
 * Range:     u8 n <= 9694, u16 n <= 9694, u32 n <= 9694, u64 n <= 9694,
 *            u128 n <= 9694, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)8;
    return (n % 5 == 0) ? (seq_t)16 : (seq_t)1;
}
