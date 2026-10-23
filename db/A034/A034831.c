/*
 * A034831  a(n) = n-th sept-factorial number divided by 4.
 * https://oeis.org/A034831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{j=1}^{n} (7*j - 3) / 4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS sequence A034831 */
seq_t a(idx_t n)
{
    /* Return 0 for n==0 for safety, though the sequence is defined for n>=1. */
    if (n == 0)
        return (seq_t)0;

    seq_t result = (seq_t)1;

    /* Multiply by each factor (7*j - 3). The term fits in a small int
       so the multiplication stays efficient even for large n. */
    for (idx_t j = 1; j <= n; ++j) {
        seq_t term = ((seq_t)7 * (seq_t)j) - (seq_t)3;
        result *= term;
    }

    /* The product is always divisible by 4; perform the integer division. */
    result /= (seq_t)4;

    return result;
}
