/*
 * A032096  "BHK" (reversible, identity, unlabeled) transform of 2,2,2,2,...
 * https://oeis.org/A032096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/6)*((-1)^n-5)*3^floor(n/2) + 3^(n-1) + 2 * Equivalent
 *            simplified form: * a(n) = 3^(n-1) + 2 - (n even ? 2*3^(n/2-1) :
 *            3^(n/2)) * for n >= 1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* offset starts at 1 */

    seq_t result = pow3(n - 1) + (seq_t)2;
    idx_t half = n / 2;

    if ((n & 1) == 0) {          /* n even */
        /* half >= 1 for n >= 2 */
        result -= (seq_t)(2 * pow3(half - 1));
    } else {                    /* n odd */
        result -= pow3(half);
    }

    return result;
}
