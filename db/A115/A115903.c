/*
 * A115903  Expansion of (1-12*x)^(-3/2).
 * https://oeis.org/A115903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n*(2*n+1)*binomial(2*n,n). Recurrence: a(0) = 1, and
 *            for n ≥ 1 a(n) = 6*(2*n+1)*a(n-1) / n (exact division)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t val = 1;                    /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t prev = val;            /* a(i-1) */
        seq_t div = prev / (seq_t)i; /* exact division */
        seq_t mult = (seq_t)(6 * ((2 * i) + 1));
        val = div * mult;            /* a(i) */
    }
    return val;
}
