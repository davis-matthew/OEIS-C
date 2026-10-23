/*
 * A027968  a(n) = T(n, 2*n-6), T given by A027960.
 * https://oeis.org/A027968
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-7920 +7548*n -3176*n^2 +735*n^3 -65*n^4 -3*n^5
 *            +n^6)/720
 * Range:     u8 none, u16 n <= 7, u32 n <= 120, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;            /* sequence defined only for n ≥ 3 */

    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;
    seq_t n3  = n2 * N;
    seq_t n4  = n3 * N;
    seq_t n5  = n4 * N;
    seq_t n6  = n5 * N;

    /* positive part of the polynomial */
    seq_t pos = n6 + (seq_t)735 * n3 + (seq_t)7548 * N;

    /* negative part of the polynomial */
    seq_t neg = (seq_t)3   * n5
              + (seq_t)65  * n4
              + (seq_t)3176 * n2
              + (seq_t)7920;

    seq_t num = pos - neg;   /* safe: pos ≥ neg for all n ≥ 3 */
    return num / 720;
}
