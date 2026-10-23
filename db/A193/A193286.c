/*
 * A193286  a(n) is the maximal number of a's that can be produced in a blank
 *          document with n "keystrokes".
 * https://oeis.org/A193286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5, a(6)=6, a(7)=7, a(8)=9;
 *            For 9 ≤ n ≤ 27: C = floor((n+3)/6), R = (n+2) mod C, Q =
 *            floor((n+2)/C)-2 a(n) = Q^(C-R) * (Q+1)^R For n ≥ 28: C =
 *            floor((n+2)/6), R = (n+2) mod 6 a(n) = 4^(C-R) * 5^R (For n ≥ 34
 *            one may also use a(n)=4·a(n−6).)
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 93, u64 n <= 189,
 *            u128 n <= 381, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Handle the small values explicitly */
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 3;
    if (n == 4) return 4;
    if (n == 5) return 5;
    if (n == 6) return 6;
    if (n == 7) return 7;
    if (n == 8) return 9;

    /* 9 ≤ n ≤ 27 */
    if (n <= 27) {
        idx_t C = (n + 3) / 6;               /* floor((n+3)/6)  */
        idx_t R = (n + 2) % C;               /* (n+2) mod C     */
        idx_t Q = (n + 2) / C - 2;           /* floor((n+2)/C)-2*/

        seq_t part1 = seq_pow((seq_t)Q, C - R);
        seq_t part2 = seq_pow((seq_t)(Q + 1), R);
        return part1 * part2;
    }

    /* n ≥ 28 */
    idx_t C = (n + 2) / 6;                   /* floor((n+2)/6)  */
    idx_t R = (n + 2) % 6;                   /* (n+2) mod 6     */

    seq_t part1 = seq_pow((seq_t)4, C - R);
    seq_t part2 = seq_pow((seq_t)5, R);
    return part1 * part2;
}
