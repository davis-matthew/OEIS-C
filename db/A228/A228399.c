/*
 * A228399  The number of permutations of length n sortable by 2 cut-and-paste
 *          moves.
 * https://oeis.org/A228399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! for 1 ≤ n < 5; a(n) = -18 + n*(107*n^5 - 1077*n^4 +
 *            2225*n^3 + 12345*n^2 - 61732*n + 80532)/720 for n ≥ 5
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 20, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* For n < 5 just return the factorial */
    if (n < 5)
        return factorial(n);

    /* Compute the polynomial part using 128‑ or arbitrary‑precision multiplications */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;        /* n^2 */
    seq_t n3 = n2 * nn;        /* n^3 */
    seq_t n4 = n3 * nn;        /* n^4 */
    seq_t n5 = n4 * nn;        /* n^5 */

    /* 107*n^5 - 1077*n^4 + 2225*n^3 + 12345*n^2 - 61732*n + 80532 */
    seq_t poly = 107 * n5
               - 1077 * n4
               + 2225 * n3
               + 12345 * n2
               - 61732 * nn
               + 80532;

    /* n * poly / 720  */
    seq_t val = (nn * poly) / 720;

    return val - 18;
}
