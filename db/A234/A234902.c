/*
 * A234902  a(n)*Pi is the total length of irregular spiral (center points: 1,
 *          2, 3) after n rotations.
 * https://oeis.org/A234902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 5*floor(n/5) + 2*floor((n+1)/5) + 2*floor((n+2)/5)
 *            + 5*floor((n+3)/5)
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t r = N * (seq_t)2;
    r += (seq_t)5 * (N / (seq_t)5);
    r += (seq_t)2 * ((N + (seq_t)1) / (seq_t)5);
    r += (seq_t)2 * ((N + (seq_t)2) / (seq_t)5);
    r += (seq_t)5 * ((N + (seq_t)3) / (seq_t)5);
    return r;
}
