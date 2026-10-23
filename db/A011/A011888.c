/*
 * A011888  Partial sums of A011863.
 * https://oeis.org/A011888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+3)*(6*n^4+36*n^3+76*n^2+66*n+5)/960 - (-1)^n/64
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 201, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the expression using a 128‑bit integer for safety */
    unsigned __int128 nn = n;          /* cast index to 128 bit */
    unsigned __int128 term1 = 2*nn + 3; /* (2*n+3) */

    /* powers of n */
    unsigned __int128 n2 = nn * nn;     /* n^2 */
    unsigned __int128 n3 = n2 * nn;     /* n^3 */
    unsigned __int128 n4 = n3 * nn;     /* n^4 */

    /* polynomial part: 6*n^4+36*n^3+76*n^2+66*n+5 */
    unsigned __int128 poly = 6*n4 + 36*n3 + 76*n2 + 66*nn + 5;

    /* numerator: (2*n+3)*poly - (-1)^n*15 */
    unsigned __int128 numerator = term1 * poly;
    if (n & 1)         /* n odd : (-1)^n = -1  -> subtract(-15) = +15 */
        numerator += 15;
    else                /* n even : (-1)^n = +1 -> subtract(15)  */
        numerator -= 15;

    /* final division by 960 */
    return (seq_t)(numerator / 960);
}
