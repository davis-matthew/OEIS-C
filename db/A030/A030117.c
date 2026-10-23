/*
 * A030117  Number of triangles a queen can make (starting anywhere) on an n X
 *          n board.
 * https://oeis.org/A030117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*binomial(n,3) + 5*binomial(n,2) - (n even ? n/2 : 0)
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 1256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)                      /* a(0) = a(1) = 0 */
        return 0;

    seq_t nn = n;

    /* nC2 = n*(n-1)/2  */
    seq_t d = nn;
    seq_t e = nn - 1;
    if (d % (seq_t)2 == 0)
        d /= 2;
    else
        e /= 2;
    seq_t c2 = d * e;

    /* nC3 = n*(n-1)*(n-2)/6  */
    seq_t a1 = nn;
    seq_t b1 = nn - 1;
    seq_t c1 = nn - 2;

    /* divide by 3 */
    if (a1 % (seq_t)3 == 0)
        a1 /= 3;
    else if (b1 % (seq_t)3 == 0)
        b1 /= 3;
    else
        c1 /= 3;

    /* divide by 2 */
    if (a1 % (seq_t)2 == 0)
        a1 /= 2;
    else if (b1 % (seq_t)2 == 0)
        b1 /= 2;
    else
        c1 /= 2;

    seq_t c3 = a1 * b1 * c1;

    /* assemble result */
    seq_t result = (seq_t)13 * c3 + (seq_t)5 * c2;
    if (nn % (seq_t)2 == 0)          /* n is even */
        result -= nn / 2;

    return result;
}
