/*
 * A131337  Row sums of triangle A131336.
 * https://oeis.org/A131337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = a(n-1) + 3*a(n-2) - a(n-3) - 3*a(n-4) - a(n-5), * with
 *            initial values a(1)=1, a(2)=2, a(3)=4, a(4)=9, a(5)=16. * The
 *            sequence satisfies the generating function * x*(1 + x - x^2)/((1
 *            + x)*(1 - x - x^2)^2)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 41, u64 n <= 85, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset is 1; behavior for 0 is arbitrary */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)9;
    if (n == 5) return (seq_t)16;

    seq_t a1 = (seq_t)1;  /* a(n-5)   */
    seq_t a2 = (seq_t)2;  /* a(n-4)   */
    seq_t a3 = (seq_t)4;  /* a(n-3)   */
    seq_t a4 = (seq_t)9;  /* a(n-2)   */
    seq_t a5 = (seq_t)16; /* a(n-1)   */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t an = a5 + (seq_t)3 * a4 - a3 - (seq_t)3 * a2 - a1; /* a(i) */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }
    return a5;
}
