/*
 * A220092  a(n) = ((2*n-1)!! + (-1)^((n-1)*(n-2)/2))/2.
 * https://oeis.org/A220092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n-1)!! + (-1)^((n-1)*(n-2)/2))/2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the double factorial (2*n-1)!! */
    seq_t prod = (seq_t)1;
    for (idx_t k = 3; k <= 2*n - 1; k += 2)
        prod *= (seq_t)k;

    /* Determine the sign of (-1)^((n-1)*(n-2)/2) */
    int sign = ((n % 4) == 1 || (n % 4) == 2) ? 1 : -1;

    /* Add or subtract 1 accordingly */
    if (sign > 0)
        prod += (seq_t)1;
    else
        prod -= (seq_t)1;

    /* Divide by 2 */
    prod /= (seq_t)2;

    return prod;
}
