/*
 * A008751  Expansion of (1+x^8)/((1-x)*(1-x^2)*(1-x^3)).
 * https://oeis.org/A008751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2-2*n+18)/6) for n > 2, with a(0)=1, a(1)=1,
 *            a(2)=2
 * Range:     u8 n <= 16, u16 n <= 373, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return 1;
    if (n == 2) return 2;

    seq_t nn = (seq_t)n;             /* promote index to the sequence type */
    seq_t val = nn * nn - 2 * nn + 18;
    return val / 6;                  /* integer division is floor for unsigned */
}
