/*
 * A180576  Wiener index of the n-web graph.
 * https://oeis.org/A180576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n*(9*n+40)+9*(-1)^n-25)/16
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent piecewise formulas are used for safe integer arithmetic. */
seq_t a(idx_t n)
{
    /* n is always >= 1 in the OEIS offset; safety for n==0 is harmless. */
    if (n == 0)
        return (seq_t)0;

    if ((n & 1) == 0) {
        /* n even:  n = 2*k,  a = k * (9*k*k + 20*k - 2) */
        seq_t k = (seq_t)(n / 2);
        seq_t k2 = k * k;
        seq_t part = ((seq_t)9 * k2) + ((seq_t)20 * k) - (seq_t)2;
        return k * part;
    } else {
        /* n odd:  n = 2*k + 1,  a = (2*k + 1) * (9*k*k + 29*k + 8) / 2 */
        seq_t k = (seq_t)((n - 1) / 2);
        seq_t k2 = k * k;
        seq_t part = ((seq_t)9 * k2) + ((seq_t)29 * k) + (seq_t)8;
        seq_t mult = ((seq_t)(2 * k + 1)) * part;
        return mult / (seq_t)2;
    }
}
