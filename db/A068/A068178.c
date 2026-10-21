/*
 * A068178  a(1) = 4; for n>1, a(n) is the smallest square > a(n-1) obtained
 *          by inserting digits into a(n-1).
 * https://oeis.org/A068178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=4; a(2)=49; a(3)=1849; for n>=4: let t = n-4. Then a(n) =
 *            ((10^t + (10^t-1)/9*7)*100 + 84)*10^t + (10^t-1)/9*8)*100 + 96
 *            which yields the digit pattern 1, t×'7', 8, 4, t×'8', 9, 6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1) return 4;
    if (n == 2) return 49;
    if (n == 3) return 1849;

    idx_t t = n - 4;                 /* number of 7's and 8's to insert */

    seq_t ten_t = pow10(t);          /* 10^t */
    seq_t sub   = (ten_t - 1) / 9;   /* (10^t - 1)/9 */

    seq_t prefix = ten_t + sub * 7;          /* 1 followed by t 7's */
    seq_t a2     = prefix * 100 + 84;        /* append 8 and 4 */

    seq_t onesum_8_t = sub * 8;              /* sum of t 8's */
    seq_t b = a2 * ten_t + onesum_8_t;      /* append t 8's */

    seq_t result = b * 100 + 96;            /* append 9 and 6 */

    return result;
}
