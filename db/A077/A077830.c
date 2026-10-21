/*
 * A077830  Expansion of 1/(1-3*x-2*x^2-3*x^3).
 * https://oeis.org/A077830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) + 3*a(n-3) a(0) = 1, a(1) = 3, a(2) =
 *            11 (expansion of 1/(1-3x-2x²-3x³) )
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first three terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)11;

    /* rolling computation for the linear recurrence */
    seq_t a0 = (seq_t)1;        /* a(0) */
    seq_t a1 = (seq_t)3;        /* a(1) */
    seq_t a2 = (seq_t)11;       /* a(2) */
    seq_t a3;
    idx_t i;

    for (i = 3; i <= n; ++i) {
        a3 = ((seq_t)3 * a2) + ((seq_t)2 * a1) + ((seq_t)3 * a0);
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    return a2;
}
