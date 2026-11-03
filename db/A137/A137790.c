/*
 * A137790  a(n) = 8^n-7^n-6^n-5^n-4^n-3^n-2^n.
 * https://oeis.org/A137790
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n-7^n-6^n-5^n-4^n-3^n-2^n
 * Range:     u8 none, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 22 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow.h"

/* Computes a(n) = 8^n - 7^n - 6^n - 5^n - 4^n - 3^n - 2^n.
   For n < 5 the value is negative; the function returns 0 in that case,
   but the sequence is defined only for n ≥ 5. */
seq_t a(idx_t n)
{
    if (n < 5) return 0;          /* sequence is defined only for n ≥ 5 */

    seq_t exp = (seq_t)n;          /* exponent cast to seq_t */

    seq_t pow8 = ipow((seq_t)8, exp);

    seq_t sum = 0;
    sum += ipow((seq_t)2, exp);
    sum += ipow((seq_t)3, exp);
    sum += ipow((seq_t)4, exp);
    sum += ipow((seq_t)5, exp);
    sum += ipow((seq_t)6, exp);
    sum += ipow((seq_t)7, exp);

    return pow8 - sum;
}
