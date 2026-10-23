/*
 * A178946  a(n) = n*(n+1)*(2*n+1)/6 - n*floor(n/2).
 * https://oeis.org/A178946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)*(2*n+1)/6 - n*floor(n/2)
 * Range:     u8 n <= 9, u16 n <= 58, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset starts at 1, but safeguard 0 */

    /* cast index to seq_t for all arithmetic */
    seq_t ns = (seq_t)n;

    /* components of the triangular sum of squares */
    seq_t a1 = ns;          /* n       */
    seq_t a2 = ns + 1;      /* n + 1   */
    seq_t a3 = ns + ns + 1; /* 2*n + 1 */

    /* divide the product a1*a2*a3 by 6 = 2*3.
       Perform the divisions first to avoid overflow in the product.
       Since the product is divisible by 6, one of the factors is even
       and one is a multiple of 3. */

    /* divide by 2 */
    if ((a1 % 2) == 0)
        a1 /= 2;
    else if ((a2 % 2) == 0)
        a2 /= 2;
    else
        a3 /= 2;

    /* divide by 3 */
    if ((a1 % 3) == 0)
        a1 /= 3;
    else if ((a2 % 3) == 0)
        a2 /= 3;
    else
        a3 /= 3;

    /* sum of first n squares */
    seq_t sumsq = a1 * a2 * a3;

    /* subtract n * floor(n/2) */
    seq_t sub = ns * (ns / 2);

    return sumsq - sub;
}
