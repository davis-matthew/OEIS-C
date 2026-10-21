/*
 * A082587  G.f.: (3+x+x^2+2*x^3)/(1-x^2-x^4).
 * https://oeis.org/A082587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-4) with a(0)=3, a(1)=1, a(2)=4, a(3)=3
 * Range:     u8 n <= 19, u16 n <= 43, u32 n <= 89, u64 n <= 181,
 *            u128 n <= 365, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for n = 0,1,2,3 */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)3;

    /* Use a small ring buffer of size 4 to hold the last computed terms. */
    seq_t buf[4] = { (seq_t)3, (seq_t)1, (seq_t)4, (seq_t)3 };

    /* Compute terms from 4 up to n inclusive. */
    for (idx_t i = 4; i <= n; ++i) {
        /* Indexing modulo 4 is safe because i >= 4. */
        buf[i & 3] = buf[(i - 2) & 3] + buf[(i - 4) & 3];
    }

    /* Return the term corresponding to the requested index. */
    return buf[n & 3];
}
