/*
 * A076301  Related to number of labeled partially ordered sets.
 * https://oeis.org/A076301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4 - n + n^2) * n! / 4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small n handled directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)15;

    /* For n ≥ 4, n! contains a factor 4.  Divide that factor out
       by simply skipping the multiplier 4 in the factorial loop. */
    seq_t res = 1;
    for (idx_t i = 2; i <= n; ++i) {
        if (i == 4) continue;            /* skip the factor 4 */
        res *= (seq_t)i;                  /* incremental factorial part  / 4 */
    }

    /* Multiply by the polynomial part (n^2 - n + 4). */
    seq_t coeff = (seq_t)n * (seq_t)n - (seq_t)n + (seq_t)4;
    res *= coeff;
    return res;
}
