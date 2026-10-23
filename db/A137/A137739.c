/*
 * A137739  Number of different strings of length n+6 obtained from "123...n"
 *          by iteratively duplicating any substring.
 * https://oeis.org/A137739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/720*(n+9)*(n^5+36*n^4+451*n^3+1716*n^2-380*n-8880)-1
 *            for n>4 a(1) = 1, a(2) = 64, a(3) = 355, a(4) = 1014
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 1;
    if (n == 2) return 64;
    if (n == 3) return 355;
    if (n == 4) return 1014;

    seq_t nn   = (seq_t)n;
    seq_t n2   = nn * nn;                 /* n^2 */
    seq_t n3   = n2 * nn;                 /* n^3 */
    seq_t n4   = n3 * nn;                 /* n^4 */
    seq_t n5   = n4 * nn;                 /* n^5 */

    seq_t expr = n5;
    expr += 36 * n4;
    expr += 451 * n3;
    expr += 1716 * n2;
    expr -= 380 * nn;
    expr -= (seq_t)8880;

    seq_t num = (nn + 9) * expr;          /* (n+9)*expr */
    seq_t div = num / 720;                /* division by 720 */
    return div - 1;
}
