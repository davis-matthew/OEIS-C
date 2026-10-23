/*
 * A014925  Number of zeros in numbers 1 to 111...1 (n+1 digits).
 * https://oeis.org/A014925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n*10^(n-1) + a(n-1), a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)           /* not used in OEIS offset, but safe */
        return 0;

    seq_t result = (seq_t)1;    /* a(1) = 1 */
    if (n == 1)
        return result;

    seq_t power10 = (seq_t)10;  /* 10^(2-1) for the first iteration   */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* term_i = i * 10^(i-1) + a(i-1)  */
        seq_t term = power10 * (seq_t)i + result;
        result = term;

        /* prepare power10 for next iteration: 10^i */
        power10 *= (seq_t)10;
    }

    return result;
}
