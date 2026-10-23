/*
 * A192828  Molecular topological indices of the n X n grid graphs.
 * https://oeis.org/A192828
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(2+n)*(6 -24*n +25*n^2 -13*n^3 +4*n^4)/3
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 69, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For indices below the offset the value is undefined – return 0 */
    if (n < 2)
        return 0;

    seq_t nn = (seq_t)n;       /* copy n into the target type */
    seq_t n2 = nn * nn;        /* n^2 */
    seq_t n3 = n2 * nn;        /* n^3 */
    seq_t n4 = n2 * n2;        /* n^4 */

    /* polynomial part: 6 -24*n +25*n^2 -13*n^3 +4*n^4 */
    seq_t p = 6
            - (seq_t)24 * nn
            + (seq_t)25 * n2
            - (seq_t)13 * n3
            + (seq_t)4  * n4;

    /* Reduce risk of overflow: divide by 3 early */
    seq_t np2 = nn + (seq_t)2;          /* n + 2 */
    seq_t res;
    if ((np2 % 3) == 0)                 /* n+2 is divisible by 3 */
    {
        seq_t factor = np2 / 3;
        res = 2 * factor * p;          /* 2 * ((n+2)/3) * p */
    }
    else                                 /* otherwise p is divisible by 3 */
    {
        seq_t factor = p / 3;
        res = 2 * np2 * factor;        /* 2 * (n+2) * (p/3) */
    }
    return res;
}
