/*
 * A105058  Expansion of g.f. (1+8*x-x^2)/((1+x)*(1-6*x+x^2)).
 * https://oeis.org/A105058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = P(2*n+2) - (-1)^n, where Pell numbers P satisfy P(0)=0,
 *            P(1)=1, P(k)=2*P(k-1)+P(k-2). This yields the OEIS sequence
 *            A105058 with offset 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the Pell number P(2*n+2). */
    seq_t p_prev = 0;     /* P(k-2)  */
    seq_t p_curr = 1;     /* P(k-1)  */
    seq_t p_next;
    /* m = 2*n + 2, guaranteed to be >= 2 for n >= 0 */
    idx_t m = n * 2 + 2;

    for (idx_t i = 2; i <= m; ++i) {
        p_next = p_curr * 2 + p_prev;
        p_prev = p_curr;
        p_curr = p_next;
    }

    /* p_curr now holds P(m) = P(2*n+2).  Adjust by (-1)^n. */
    if (n & 1) {
        /* n is odd: (-1)^n = -1, so subtract (-1) => add 1. */
        p_curr = p_curr + 1;
    } else {
        /* n is even: (-1)^n = 1, so subtract 1. */
        p_curr = p_curr - 1;
    }
    return p_curr;
}
