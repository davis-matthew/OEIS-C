/*
 * A053052  Append n to the previous term, reverse alternate terms.
 * https://oeis.org/A053052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = if n is odd then concatenate 1,2,…,n else concatenate
 *            n,n-1,…,1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-01)
 * Helpers:   num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/num_digits0.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t res = 0;
    if ((n & (idx_t)1) == (idx_t)1) {               /* odd n: ascending */
        for (idx_t i = 1; i <= n; ++i) {
            idx_t d = (idx_t)num_digits0((uint64_t)i);
            res = res * pow10(d) + (seq_t)i;
        }
    } else {                                       /* even n: descending */
        for (idx_t i = n; i > 0; --i) {
            idx_t d = (idx_t)num_digits0((uint64_t)i);
            res = res * pow10(d) + (seq_t)i;
        }
    }
    return res;
}
