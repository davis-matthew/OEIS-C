/*
 * A045119  Numbers whose base-4 representation contains exactly two 1's and
 *          no 3's.
 * https://oeis.org/A045119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-4 representation contains exactly two 1's and no 3's
   OEIS A045119.  The function returns a(n) for n ≥ 1 (offset 1). */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for offset 1 */
    seq_t cur = 5;                       /* first term is 5 (base-4 11) */
    idx_t cnt = 1;
    if (n == 1) return cur;              /* a(1) = 5 */

    for (;;) {
        ++cur;                           /* check successive integers */
        seq_t tmp = cur;
        unsigned ones = 0;
        bool hasThree = false;

        /* examine base-4 digits using bit operations (4 = 2^2) */
        while (tmp) {
            unsigned digit = (unsigned)(tmp & 3);  /* tmp % 4 */
            if (digit == 3) { hasThree = true; break; }
            if (digit == 1) ++ones;
            tmp >>= 2;                     /* tmp /= 4 */
        }

        if (!hasThree && ones == 2) {      /* valid term */
            ++cnt;
            if (cnt == n) return cur;      /* found requested index */
        }
    }
}
