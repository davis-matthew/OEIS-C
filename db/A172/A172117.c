/*
 * A172117  a(n) = n*(n+1)*(20*n-17)/6.
 * https://oeis.org/A172117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(20*n-17)/6
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first term */
    if (n == 0U)
        return (seq_t)0;

    /* Convert index to seq_t */
    seq_t nn = (seq_t)n;
    seq_t n1 = nn + (seq_t)1;
    seq_t t3 = (seq_t)20 * nn - (seq_t)17;

    /* Reduce the product by 2 and 3 to avoid overflow */
    if (nn % (seq_t)2 == (seq_t)0)
        nn = nn / (seq_t)2;
    else if (n1 % (seq_t)2 == (seq_t)0)
        n1 = n1 / (seq_t)2;
    else
        t3 = t3 / (seq_t)2;

    if (nn % (seq_t)3 == (seq_t)0)
        nn = nn / (seq_t)3;
    else if (n1 % (seq_t)3 == (seq_t)0)
        n1 = n1 / (seq_t)3;
    else
        t3 = t3 / (seq_t)3;

    /* Compute the result */
    seq_t result = nn;
    result *= n1;
    result *= t3;
    return result;
}
