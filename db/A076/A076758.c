/*
 * A076758  a(n) = n*(n+1)^2*(2+n)*(3+2*n)*(19+8*n)/180.
 * https://oeis.org/A076758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)^2*(2+n)*(3+2*n)*(19+8*n)/180
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a wider integer type for intermediate calculations to avoid overflow.
       The product fits in 128 bits for all n relevant to the test set. */
    unsigned __int128 prod = (unsigned __int128)n;

    /* Multiply by (n+1) twice (i.e., (n+1)^2) */
    prod *= (unsigned __int128)(n + 1);
    prod *= (unsigned __int128)(n + 1);

    /* Multiply by (2+n) = (n+2) */
    prod *= (unsigned __int128)(n + 2);

    /* Multiply by (3+2*n) */
    prod *= (unsigned __int128)(3 + 2 * n);

    /* Multiply by (19+8*n) */
    prod *= (unsigned __int128)(19 + 8 * n);

    /* Divide by 180 */
    prod /= (unsigned __int128)180;

    return (seq_t)prod;
}
