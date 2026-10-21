/*
 * A081596  Let n = 10x + y where 0 <= y <= 9, x >= 0. Then a(n) = 5x+y.
 * https://oeis.org/A081596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - 5*floor(n/10) (equivalently a(n) = 5*floor(n/10) + (n
 *            mod 10))
 * Range:     u8 n <= 392, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;     /* convert index to the sequence integer type */
    seq_t x = nseq / 10;      /* floor(n/10) */
    return nseq - 5 * x;      /* a(n) = n - 5*floor(n/10) */
}
