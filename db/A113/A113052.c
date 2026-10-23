/*
 * A113052  Binomial(5n,n)/(4n+1) mod 5.
 * https://oeis.org/A113052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if n = (5^k-1)/4 for some k ≥ 0; otherwise 0
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) computes the sequence A113052: Binomial(5n,n)/(4n+1) mod 5.
 * Equivalently, it is the indicator of numbers of the form (5^k-1)/4. */
seq_t a(idx_t n)
{
    /* Compute 4*n+1 as a seq_t value. */
    seq_t target = (seq_t)n * 4 + 1;

    /* target must be a positive power of 5 to return 1. */
    if (target == 0)                /* cannot happen for n >= 0, but keep safe */
        return 0;

    /* Divide by 5 until remainder is non‑zero. */
    while (target % 5 == 0)
        target /= 5;

    /* If the remaining value is 1, target was exactly 5^k. */
    return (target == 1) ? 1 : 0;
}
