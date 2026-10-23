/*
 * A024221  [ (4th elementary symmetric function of S(n))/(first elementary
 *          symmetric function of S(n)) ], where S(n) = {first n+3 positive
 *          integers congruent to 1 mod 3}.
 * https://oeis.org/A024221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( e4(S(n)) / e1(S(n)) ) where S(n) = {1, 4, 7, 10,
 *            …, 3 (n+2)+1} is the first n+3 positive integers congruent to 1
 *            mod 3
 * Range:     u8 none, u16 n <= 2, u32 n <= 14, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Number of elements in S(n) is n + 3.  Last index is n+2. */
    idx_t m = n + 2;          /* last index (inclusive)   */
    seq_t sum1 = 0;           /* sum of elements           */
    seq_t sum2 = 0;           /* sum of squares            */
    seq_t sum3 = 0;           /* sum of cubes              */
    seq_t sum4 = 0;           /* sum of fourth powers      */

    for (idx_t i = 0; i <= m; ++i)
    {
        seq_t x  = ((seq_t)3 * i) + 1;   /* element s_i = 3i+1 */
        seq_t x2 = x * x;                /* x^2 */
        seq_t x3 = x2 * x;               /* x^3 */
        seq_t x4 = x3 * x;               /* x^4 */

        sum1 += x;
        sum2 += x2;
        sum3 += x3;
        sum4 += x4;
    }

    /* Newton's identities:  e1 = p1,  e2 = (e1*p1 - p2)/2,
       e3 = (p3 - e1*p2 + e2*p1)/3,
       e4 = (e3*p1 - e2*p2 + e1*p3 - p4)/4   */
    seq_t e1 = sum1;
    seq_t e2 = (e1 * e1 - sum2) / 2;
    seq_t e3 = (sum3 - e1 * sum2 + e2 * e1) / 3;
    seq_t e4 = (e3 * e1 - e2 * sum2 + e1 * sum3 - sum4) / 4;

    return e4 / e1;   /* floor of the ratio */
}
