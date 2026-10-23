/*
 * A141478  a(n) = binomial(n+2,3)*4^3.
 * https://oeis.org/A141478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*n*(n+1)*(n+2)/3
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 737, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the sequence type first */
    seq_t nt = (seq_t)n;

    /* Compute n*(n+1)*(n+2) */
    seq_t val = nt * (nt + 1);
    val = val * (nt + 2);

    /* Divide by 3 (the product is always divisible by 3) */
    val = val / 3;

    /* Multiply by 32 to finish the formula */
    val = val * 32;

    return val;
}
