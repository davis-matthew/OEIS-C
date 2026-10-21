/*
 * A098297  Member r=12 of the family of Chebyshev sequences S_r(n) defined in
 *          A092184.
 * https://oeis.org/A098297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - a(n-2) + 2, a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Use a simple iterative recurrence; no dynamic allocation needed. */
    seq_t prev2 = (seq_t)0;  /* a(0) */
    seq_t prev1 = (seq_t)1;  /* a(1) */
    seq_t cur = prev1;       /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        cur = ((seq_t)10 * prev1) - prev2 + (seq_t)2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
