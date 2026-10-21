/*
 * A206339  Number of (n+1) X 5 0..3 arrays with every 2 X 2 subblock having
 *          nonzero determinant and commuting with every horizontal or
 *          vertical neighbor.
 * https://oeis.org/A206339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+3*a(n-2)-3*a(n-3) (for n>13); a(1)=296, a(2)=534,
 *            a(3)=682, a(4)=1581, a(5)=2034, a(6)=4711, a(7)=6078,
 *            a(8)=14101, a(9)=18210, a(10)=42271, a(11)=54606, a(12)=126799,
 *            a(13)=163806
 * Range:     u8 none, u16 n <= 11, u32 n <= 31, u64 n <= 71, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static const seq_t init[14] = {
        0ULL,           /* index 0, unused */
        296ULL, 534ULL, 682ULL, 1581ULL, 2034ULL, 4711ULL,
        6078ULL, 14101ULL, 18210ULL, 42271ULL, 54606ULL,
        126799ULL, 163806ULL
    };

    if (n == 0)
        return 0;
    if (n <= 13)
        return init[n];

    seq_t prev3 = init[11];   /* a(11) */
    seq_t prev2 = init[12];   /* a(12) */
    seq_t prev1 = init[13];   /* a(13) */
    seq_t an;
    idx_t i;

    for (i = 14; i <= n; ++i) {
        an = prev1 + 3 * prev2 - 3 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = an;
    }
    return prev1;
}
