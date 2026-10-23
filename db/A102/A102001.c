/*
 * A102001  A weighted tribonacci, (1,2,4).
 * https://oeis.org/A102001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 4*a(n-3); with a(1)=1, a(2)=3, a(3)=9
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 49, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)9;

    seq_t v3 = (seq_t)1;    /* a(1) */
    seq_t v2 = (seq_t)3;    /* a(2) */
    seq_t v1 = (seq_t)9;    /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        /* next = v1 + 2*v2 + 4*v3, implemented with additions only */
        seq_t next = v1 + v2 + v2 + v3 + v3 + v3 + v3;
        v3 = v2;
        v2 = v1;
        v1 = next;
    }
    return v1;
}
