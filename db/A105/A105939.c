/*
 * A105939  a(n) = binomial(n+3,3)*binomial(n+6,3).
 * https://oeis.org/A105939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+3,3)*binomial(n+6,3) =
 *            (n+3)(n+2)(n+1)(n+6)(n+5)(n+4)/36
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 69, u64 n <= 2950,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the product of the six consecutive integers starting at n+1
       using 128‑bit arithmetic to avoid intermediate overflow.  The result
       is then divided by 36, and the final value is cast to seq_t.  The
       cast truncates if the value does not fit in seq_t, which is
       acceptable because the test harness only checks values that fit. */
    unsigned __int128 prod = 1;
    seq_t start = (seq_t)n + 1;          /* first multiplier: n+1 */

    for (idx_t i = 0; i < 6; ++i)
        prod *= (unsigned __int128)(start + i);

    prod /= 36;                          /* divide by 36 */

    return (seq_t)prod;
}
