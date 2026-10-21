/*
 * A206809  a(n) = Sum_{0<j<k<=n} k^3-j^3.
 * https://oeis.org/A206809
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{0<j<k<=n} (k^3 - j^3) = Sum_{k=2..n} [ k^3*(k-1) -
 *            ((k-1)*k/2)^2 ]
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 123, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the OEIS offset is not defined; return 0 (irrelevant). */
    if (n < 2) return (seq_t)0;

    seq_t sum = (seq_t)0;

    for (idx_t k = 2; k <= n; ++k) {
        seq_t kt = (seq_t)k;
        /* k^3 * (k-1) */
        seq_t term1 = kt * kt * kt * (kt - (seq_t)1);

        /* ((k-1)*k/2)^2  */
        seq_t km1 = kt - (seq_t)1;
        seq_t tri = km1 * kt / (seq_t)2;
        seq_t term2 = tri * tri;

        sum += term1 - term2;
    }
    return sum;
}
