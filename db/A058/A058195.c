/*
 * A058195  Areas of a sequence of right-angled figures described below.
 * https://oeis.org/A058195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((2*n^4 + 10*n^3 + 13*n^2 + 2*n)/24)
 * Range:     u8 n <= 6, u16 n <= 28, u32 n <= 475, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate to avoid overflow when n is large. */
    __int128 N  = n;                                     /* (seq_t)n would overflow before the mults */
    __int128 num = 2*N*N*N*N
                 + 10*N*N*N
                 + 13*N*N
                 + 2*N;                                 /* 2n⁴ + 10n³ + 13n² + 2n */
    return (seq_t)(num / 24);
}
