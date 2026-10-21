/*
 * A097657  Fibonacci sequence with first two terms 11 and 23.
 * https://oeis.org/A097657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 11, a(1) = 23
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial conditions directly */
    if (n == 0u)
        return (seq_t)11;
    if (n == 1u)
        return (seq_t)23;

    /* Iteratively compute the Fibonacci‑style sequence up to n. */
    seq_t prev0 = (seq_t)11;   /* a(n-2)   */
    seq_t prev1 = (seq_t)23;   /* a(n-1)   */

    for (idx_t i = 2u; i <= n; ++i) {
        seq_t tmp = prev0 + prev1;  /* a(i) = a(i-2) + a(i-1) */
        prev0 = prev1;              /* shift for next iteration */
        prev1 = tmp;
    }

    return prev1;   /* a(n) after the loop */
}
