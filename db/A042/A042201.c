/*
 * A042201  Denominators of continued fraction convergents to sqrt(626).
 * https://oeis.org/A042201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 50*a(n-1) + a(n-2), a(0)=1, a(1)=50
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)50;

    seq_t prev0 = (seq_t)1;        /* a(n-2) */
    seq_t prev1 = (seq_t)50;       /* a(n-1) */
    seq_t next;

    for (idx_t i = 2; i <= n; ++i) {
        next = ((seq_t)50) * prev1 + prev0;
        prev0 = prev1;
        prev1 = next;
    }
    return prev1;
}
