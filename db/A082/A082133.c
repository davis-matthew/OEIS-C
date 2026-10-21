/*
 * A082133  Expansion of e.g.f. x*exp(2*x)*cosh(x).
 * https://oeis.org/A082133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(1 + 3^(n‑1))/2, for n ≥ 0 (OEIS A082133)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* a(0) = 0 */

    /* compute 3^(n-1) */
    seq_t pow3 = 1;
    const seq_t three = 3;
    for (idx_t i = 1; i < n; ++i) {
        pow3 *= three;
    }

    /* sum = 1 + 3^(n-1) */
    seq_t sum = pow3 + 1;

    /* use division before multiplication to avoid overflow */
    seq_t result;
    if (n & 1) {                         /* n is odd: sum is even */
        result = (seq_t)n * (sum / 2);
    } else {                             /* n is even: n/2 is integer */
        result = ((seq_t)n / 2) * sum;
    }
    return result;
}
