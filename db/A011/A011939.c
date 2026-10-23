/*
 * A011939  a(n) = floor( n*(n-1)*(n-2)*(n-3)/29 ).
 * https://oeis.org/A011939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)*(n-3)/29)
 * Range:     u8 n <= 10, u16 n <= 38, u32 n <= 420, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 4 the product contains a zero factor, so a(n) = 0 */
    if (n < 4)
        return 0;

    /* Compute the product using seq_t operands to avoid intermediate
       signed‑overflow and to ensure the division is performed on the
       appropriate type.  The guard above prevents unsigned underflow
       when n < 3. */
    seq_t n_s = (seq_t)n;
    seq_t prod = n_s * ((seq_t)n - 1) * ((seq_t)n - 2) * ((seq_t)n - 3) / 29;

    return prod;
}
