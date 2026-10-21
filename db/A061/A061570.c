/*
 * A061570  a(1)=0, a(2)=1, a(n)=3*n-1 for n >= 3.
 * https://oeis.org/A061570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=0, a(2)=1, a(n)=3*n-1 for n >= 3
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined for n < 1, but return 0 just in case */
    if (n == 0) return 0;

    if (n == 1) return 0;
    if (n == 2) return 1;

    /* Compute with seq_t to avoid overflow of idx_t */
    return (seq_t)3 * (seq_t)n - (seq_t)1;
}
