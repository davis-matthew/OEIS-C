/*
 * A054579  n^2+n modulo 17.
 * https://oeis.org/A054579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n * (n-1)) mod 17
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n and n-1 modulo 17 first.  Using (n+16) guarantees
       correct behaviour for unsigned n even when n == 0. */
    seq_t nm   = (seq_t)(n % 17);
    seq_t n1m  = (seq_t)((n + 16) % 17);
    return (nm * n1m) % 17;
}
