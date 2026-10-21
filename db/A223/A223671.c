/*
 * A223671  Number of 4Xn 0..1 arrays with rows, diagonals and antidiagonals
 *          unimodal.
 * https://oeis.org/A223671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (45*n^8 - 456*n^7 + 6608*n^6 - 2562*n^5 - 307615*n^4 * +
 *            4161486*n^3 - 21197158*n^2 + 55754532*n - 58156560) / 5040 *
 *            (for n > 6) with a(1..6) = 16, 256, 1723, 6454, 18980, 49561
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 29, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Small n are hard‑coded */
    static const seq_t base[6] = {16, 256, 1723, 6454, 18980, 49561};

    if (n <= 6)
        return base[n - 1];

    /* use 128‑bit arithmetic to avoid overflow while computing the numerator */
    signed __int128 nn = (unsigned __int128)n;
    signed __int128 n2 = nn * nn;          /* n^2 */
    signed __int128 n3 = n2 * nn;          /* n^3 */
    signed __int128 n4 = n3 * nn;          /* n^4 */
    signed __int128 n5 = n4 * nn;          /* n^5 */
    signed __int128 n6 = n5 * nn;          /* n^6 */
    signed __int128 n7 = n6 * nn;          /* n^7 */
    signed __int128 n8 = n7 * nn;          /* n^8 */

    signed __int128 num = 45 * n8
                        - 456 * n7
                        + 6608 * n6
                        - 2562 * n5
                        - 307615 * n4
                        + 4161486 * n3
                        - 21197158 * n2
                        + 55754532 * nn
                        - 58156560;

    /* 5040 = 2^4 * 3^2 * 5 * 7  (LCM of denominators) */
    seq_t result = (seq_t)(num / 5040);
    return result;
}
