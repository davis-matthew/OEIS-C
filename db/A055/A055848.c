/*
 * A055848  Expansion of (1+5*x)/(1-x)^10.
 * https://oeis.org/A055848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n+3)*C(n+8,8)/3
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute binomial coefficient C(n+8,8) multiplicatively.
     * This keeps intermediate values integer and reasonably small. */
    seq_t bin = 1;
    for (idx_t i = 1; i <= 8; ++i) {
        /* bin = bin * (n + i) / i ; the division is exact at each step. */
        bin *= (n + i);
        bin /= i;
    }

    seq_t coef = (seq_t)(2 * n + 3);
    /* Apply the factor 1/3 safely.  The expression is always integral. */
    if (coef % 3 == 0) {
        return bin * (coef / 3);
    } else {
        /* In this case bin is divisible by 3 */
        return (bin / 3) * coef;
    }
}
