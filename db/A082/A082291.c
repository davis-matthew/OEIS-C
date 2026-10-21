/*
 * A082291  Expansion of x(2 + 5x - x^2)/((1 - x)(1 - 6x + x^2)).
 * https://oeis.org/A082291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2) + 6 (offset 1, a(1)=2, a(2)=19)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not part of OEIS offset, safe fallback */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)19;

    seq_t a_prev2 = (seq_t)2;   /* a(n-2) */
    seq_t a_prev1 = (seq_t)19;  /* a(n-1) */
    seq_t a_curr  = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i) {
        a_curr = 6 * a_prev1 - a_prev2 + 6;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
