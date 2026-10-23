/*
 * A014484  Expansion of (1+2x)/(1-2x)^4 (E.g.f.).
 * https://oeis.org/A014484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)*(n+2)*(2*n+3)/6) * 2^n * n!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=10, a(n) = (2*n+9)*a(n-1) - 6*(n+1)*a(n-2) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)10;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)10;  /* a(1) */
    seq_t current;
    idx_t i;

    for (i = 2; i <= n; ++i) {
        seq_t term1 = prev1 * (seq_t)(2 * i + 9);
        seq_t term2 = prev2 * (seq_t)(6 * (i + 1));
        current = term1 - term2;
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}
