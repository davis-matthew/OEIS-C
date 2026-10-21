/*
 * A080864  Numbers k such that lpf(k)+2 = lpf(k-2), where lpf=A020639 (least
 *          prime factor).
 * https://oeis.org/A080864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A080864: a(n) = the n‑th integer k such that lpf(k)+2 = lpf(k-2). */
#include "../utils/lpf.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* no term for n = 0, offset starts at 1 */
    idx_t found = 0;               /* how many terms found so far */
    seq_t k = 3;                   /* start checking from 3 (k‑2 > 0) */

    for (;; ++k) {
        idx_t p = lpf((idx_t)k);               /* least prime factor of k   */
        idx_t q = lpf((idx_t)(k - 2));         /* least prime factor of k‑2 */
        if (p + 2 == q) {                       /* condition satisfied      */
            ++found;
            if (found == n)
                return k;                        /* return the n‑th term     */
        }
    }
}
