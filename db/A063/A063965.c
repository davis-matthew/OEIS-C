/*
 * A063965  Size of the automorphism group of the group S_n x S_n (where S_n
 *          is the symmetric group).
 * https://oeis.org/A063965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 1 * 6 if n = 2 * 8*(n!)^2 if n = 6 * 2*(n!)^2
 *            otherwise (n >= 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute n! iteratively; then square and multiply by the
 * appropriate factor (2 or 8).  The function works for any
 * seq_t type that supports the usual arithmetic operators.   */
seq_t a(idx_t n)
{
    /* Special cases from the OEIS definition */
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)6;

    /* Compute factorial(n) as a seq_t */
    seq_t fact = (seq_t)1;
    for (idx_t i = (idx_t)2; i <= n; ++i)
        fact = fact * (seq_t)i;

    /* Square the factorial */
    seq_t sq = fact * fact;

    /* Apply the multiplier: 2 for all n except n = 6 */
    if (n == (idx_t)6)
        return sq * (seq_t)8;
    else
        return sq * (seq_t)2;
}
