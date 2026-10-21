/*
 * A225691  Number of Dyck paths of semilength n avoiding the pattern U^4 D^4
 *          U D.
 * https://oeis.org/A225691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*n^3-84*n^2+124*n-84)/6 for n >= 6 with a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=5, a(4)=14, a(5)=41
 * Range:     u8 n <= 5, u16 n <= 18, u32 n <= 682, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial terms for n = 0..5 */
    static const seq_t init[6] = {1, 1, 2, 5, 14, 41};

    if (n < 6)
        return init[(size_t)n];

    seq_t nn = (seq_t)n;          /* n as seq_t */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */

    /* Numerator of the closed‑form expression:
       14*n^3 - 84*n^2 + 124*n - 84                     */
    seq_t num = 14 * n3 + 124 * nn - 84 * n2 - 84;

    return num / 6;
}
