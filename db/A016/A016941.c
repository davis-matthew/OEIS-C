/*
 * A016941  a(n) = (6*n + 2)^9.
 * https://oeis.org/A016941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 2)^9
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 22, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A016941.
 * It evaluates (6*n + 2) raised to the 9th power using
 * repeated multiplication on the integer type seq_t. */
seq_t a(idx_t n)
{
    /* Compute the base value (6*n + 2). */
    seq_t base = ((seq_t)6 * (seq_t)n) + (seq_t)2;

    /* Raise the base to the 9th power by multiplying.
     * Start with result = base and multiply it by base 8 more times. */
    seq_t result = base;
    for (int i = 1; i < 9; ++i)
    {
        result *= base;
    }

    return result;
}
