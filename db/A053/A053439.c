/*
 * A053439  Expansion of (1+x+2*x^3)/((1-x)*(1-x^2)^2).
 * https://oeis.org/A053439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n even: 2*(n/2)^2 + (n/2) + 1, * if n odd :
 *            2*((n+1)/2)^2
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute using the closed forms given for even and odd n
       Avoid intermediate overflow by performing all operations in seq_t. */
    if ((n & (idx_t)1) == 0)         /* n is even */
    {
        seq_t half = (seq_t)(n >> 1);
        return 2 * half * half + half + (seq_t)1;
    }
    else                               /* n is odd */
    {
        seq_t half = (seq_t)(n >> 1);  /* (n-1)/2 */
        seq_t m = half + (seq_t)1;     /* (n+1)/2 */
        return 2 * m * m;
    }
}
