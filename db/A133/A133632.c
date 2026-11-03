/*
 * A133632  a(1)=1, a(n) = (p-1)*a(n-1), if n is even, otherwise a(n) =
 *          p*a(n-2), where p = 5.
 * https://oeis.org/A133632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^⌊(n-1)/2⌋ * (5 + 3*(-1)^n)/2
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A133632 for p = 5 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* n >= 1 */
    idx_t exp = (n - 1) / 2;                     /* floor((n-1)/2) */
    seq_t val = ipow((seq_t)5, (seq_t)exp);   /* 5^exp */

    if ((n & 1) == 0) {                          /* n even */
        val = val * (seq_t)4;                    /* multiply by 4 */
    }
    /* for n odd val is already correct (multiplied by 1) */

    return val;
}
