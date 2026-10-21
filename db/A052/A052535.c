/*
 * A052535  Expansion of (1-x)*(1+x)/(1-x-2*x^2+x^4).
 * https://oeis.org/A052535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-4) (OEIS A052535) with initial
 *            values a(0)=1, a(1)=1, a(2)=2, a(3)=4
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 35, u64 n <= 69, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the four known initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;

    /* Use a sliding window of the last four computed terms. */
    seq_t a0 = (seq_t)1; /* a(n-4) */
    seq_t a1 = (seq_t)1; /* a(n-3) */
    seq_t a2 = (seq_t)2; /* a(n-2) */
    seq_t a3 = (seq_t)4; /* a(n-1) */
    seq_t ai;

    for (idx_t k = 4; k <= n; ++k) {
        ai = a3 + (seq_t)2 * a2 - a0; /* a(k) = a(k-1)+2*a(k-2)-a(k-4) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
