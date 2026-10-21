/*
 * A059717  Start with decimal expansion of n; if all digits have the same
 *          parity, stop; otherwise write down the number formed by the even
 *          digits and the number formed by the odd digits and add them;
 *          repeat.
 * https://oeis.org/A059717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice, look
 * Formula:   not stated
 * Range:     u8 n <= 258, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   same_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  A059717:  The sequence defined by repeatedly partitioning the decimal
 *  digits of a number into the even‑digit part and the odd‑digit part,
 *  adding those parts, and repeating until all digits have the same
 *  parity.  The final number is the value a(n).  Formally:
 *
 *  a(n) = n                      if all digits of n have the same parity
 *  a(n) = a(e(n)+o(n))           otherwise
 *  where e(n) is the number formed by the even digits of n
 *        and o(n) is the number formed by the odd digits of n.
 */

#include <stdbool.h>
#include "../utils/same_parity.h"

/* required function */
seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;

    if (cur == 0)          /* a(0) = 0 */
        return 0;

    while (!same_parity(cur)) {
        seq_t even_part = 0;
        seq_t odd_part  = 0;

        /* find the highest power of 10 <= cur */
        seq_t pow10 = 1;
        while (cur / 10 >= pow10)
            pow10 *= 10;               /* safe: cur ≤ 2^64‑1 */

        seq_t rem = cur;
        while (pow10 > 0) {
            seq_t digit = rem / pow10;
            rem = rem % pow10;
            pow10 /= 10;

            if (digit % 2 == 0)        /* even digit */
                even_part = even_part * 10 + digit;
            else                       /* odd digit */
                odd_part  = odd_part  * 10 + digit;
        }

        cur = even_part + odd_part;
    }

    return cur;
}
