/*
 * A095670  Eighth column (m=7) of (1,4)-Pascal triangle A095666.
 * https://oeis.org/A095670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+28) * binomial(n+6,6) / 7
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 58, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute C(n+6,6) multiplicatively.
       For k=6 we can use the standard recurrence:
       C(N,k) = ∏_{i=1}^{k} (N-k+i)/i
       Here N = n+6, so N-k+i = n+i.            */
    seq_t res = 1;
    for (idx_t i = 1; i <= 6; ++i) {
        seq_t num   = (seq_t)(n + i);     /* n+i fits in seq_t */
        res = res * num / i;              /* division exact for binomial */
    }
    /* Now res = C(n+6,6).  Compute (n+28)*res/7. */
    seq_t val = (seq_t)(n + 28) * res;
    val = val / 7;                         /* exact division */
    return val;
}
