/*
 * A029757  a(n) = 5^n mod 2^n.
 * https://oeis.org/A029757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 2^n
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 31, u64 n <= 64, u128 n <= 129,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(0) is defined to be 0 */
    if (n == 0) return 0;

    seq_t res = 1;

    /* For fixed‑size types we can mask with a normal integer mask;
       for arbitrary‑precision types we build the mask with pow2(). */
    if (sizeof(seq_t) <= 8) {
        /* Small width: 2^n is representable only for n < 64.
           For n >= 64 the modulus exceeds 2^64; the value then does
           not fit in seq_t anyway, so we can simply let the
           multiplication overflow to obtain the wrapped result. */
        if (n < 64) {
            seq_t mask = ((seq_t)1 << n) - 1;   /* 2^n – 1, lower n bits set */
            for (idx_t i = 0; i < n; ++i) {
                res = ((res * (seq_t)5) & mask);
            }
            return res;
        }
        /* n >= 64: use wrap–around modulo 2^64 */
        for (idx_t i = 0; i < n; ++i) res *= 5;
        return res;
    } else {
        /* Arbitrary‑precision type: compute mask = 2^n – 1 with pow2().
           The mask keeps the lower n bits, equivalent to modulo 2^n. */
        seq_t mask = pow2(n) - 1;
        for (idx_t i = 0; i < n; ++i) {
            res = (res * 5) & mask;
        }
        return res;
    }
}
