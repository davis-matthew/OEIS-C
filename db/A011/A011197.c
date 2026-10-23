/*
 * A011197  a(n) = n*(n+1)*(2*n+1)*(3*n+1)*(4*n+1)/6.
 * https://oeis.org/A011197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(2*n+1)*(3*n+1)*(4*n+1)/6
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 63, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert index to seq_t first */
    seq_t n0 = (seq_t)n;
    seq_t n1 = n0 + 1;
    seq_t n2 = 2 * n0 + 1;
    seq_t n3 = 3 * n0 + 1;
    seq_t n4 = 4 * n0 + 1;

    /* Divide out a factor of 2 (n*(n+1) is always even) */
    if ((n0 & 1) == 0)
        n0 /= 2;
    else
        n1 /= 2;

    /* Divide out a factor of 3 (overall product divisible by 3) */
    if ((n0 % 3) == 0)
        n0 /= 3;
    else if ((n1 % 3) == 0)
        n1 /= 3;
    else if ((n2 % 3) == 0)
        n2 /= 3;
    else if ((n3 % 3) == 0)
        n3 /= 3;
    else
        n4 /= 3;   /* Must be divisible here */

    /* Now multiply the five remaining factors – this is the exact value */
    seq_t res = n0;
    res *= n1;
    res *= n2;
    res *= n3;
    res *= n4;
    return res;
}
