/*
 * A170009  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A170009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 48 * 47^(n-1) for n >= 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for OEIS sequence A170009.
   The growth of the Coxeter group implies a simple recurrence
   with no adjacent equal generators, giving a closed form: */
seq_t a(idx_t n)
{
    if (n == 0)      /* a(0) = 1 */
        return (seq_t)1;

    seq_t result = (seq_t)48;   /* a(1) = 48 */
    for (idx_t i = 1; i < n; ++i) {
        result *= (seq_t)47;
    }
    return result;
}
