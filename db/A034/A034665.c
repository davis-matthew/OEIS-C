/*
 * A034665  Sum of n-th powers of divisors of 32.
 * https://oeis.org/A034665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2^n + 4^n + 8^n + 16^n + 32^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)1;                     /* term for 1^n */

    /* Bases of the powers of two */
    const int bases[] = { 2, 4, 8, 16, 32 };

    for (int b = 0; b < 5; ++b)
    {
        seq_t base = (seq_t)bases[b];
        seq_t term = (seq_t)1;

        for (idx_t i = (idx_t)0; i < n; ++i)
            term = term * base;

        sum += term;
    }

    return sum;
}
