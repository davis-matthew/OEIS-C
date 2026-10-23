/*
 * A160706  Hankel transform of A052702(n+1).
 * https://oeis.org/A160706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 385, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: 
 *   a(n) = {  (-1)^((n-3)/3) * ((n-3)/3 + 1),  if n % 3 == 0 and n >= 3
 *          , (-1)^((n-5)/3 + 1) * ((n-5)/3 + 1), if n % 3 == 2 and n >= 5
 *          , 0,                                      otherwise }
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle positions where the sequence is zero */
    if (n % 3 == 1)
        return 0;

    /* n % 3 == 0 case */
    if (n % 3 == 0) {
        if (n < 3)
            return 0;              /* indices 0,1,2 */
        idx_t k = (n - 3) / 3;      /* k >= 0 */
        seq_t val = (seq_t)(k + 1);
        if (k & 1)                 /* (-1)^k == -1 when k odd */
            val = -(val);
        return val;
    }

    /* n % 3 == 2 case */
    if (n % 3 == 2) {
        if (n < 5)
            return 0;              /* index 2 is already covered, next odd is 5 */
        idx_t k = (n - 5) / 3;      /* k >= 0 */
        seq_t val = (seq_t)(k + 1);
        /* For this case exponent is k+1, so sign is negative when k is even */
        if ((k & 1) == 0)
            val = -(val);
        return val;
    }

    return 0; /* should not reach here */
}
