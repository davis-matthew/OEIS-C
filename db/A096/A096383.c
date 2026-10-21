/*
 * A096383  Area of the Pythagorean triangle a = u^2 - v^2 (cf. A096382) when
 *          v = 3, u = 4, 5, 6, ...
 * https://oeis.org/A096383
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-3) * (3*n) * (n+3) (n ≥ 4)
 * Range:     u8 n <= 5, u16 n <= 28, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS sequence A096383.
 * For n < 4 the value is undefined by the sequence definition;
 * the function returns 0 in that case, as the behaviour below the
 * offset does not matter. */
seq_t a(idx_t n)
{
    if (n < 4)
        return 0;               /* below offset; value irrelevant */
    seq_t nn = (seq_t)n;       /* promote index to seq_t */
    return (nn - 3) * (nn * 3) * (nn + 3);
}
