/*
 * A096942  Fifth column of (1,5)-Pascal triangle A096940.
 * https://oeis.org/A096942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+20) * binomial(n+3, 3) / 4 (OEIS A096942)
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 422, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  binomial(n+3, 3) is computed multiplicatively to keep intermediates small. */
seq_t a(idx_t n)
{
    seq_t bin = 1;                         /* binomial(n+3,3)  */
    for (idx_t i = 1; i <= 3; ++i) {
        seq_t term = (seq_t)n + (seq_t)(4 - i);  /* n+3, n+2, n+1   */
        bin = bin * term / (seq_t)i;             /* exact integer division */
    }
    seq_t res = bin * ((seq_t)n + (seq_t)20);    /* multiply by (n+20) */
    res = res / (seq_t)4;                        /* divide by 4          */
    return res;
}
