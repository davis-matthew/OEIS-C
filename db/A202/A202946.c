/*
 * A202946  a(n+1) = 6*A060544(n)*a(n).
 * https://oeis.org/A202946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 3; a(n+1) = 6*A060544(n)*a(n), where A060544(n) = 1 +
 *            9*C(n,2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t val = (seq_t)3;          /* a(1) */
    for (idx_t k = 1; k < n; ++k)
    {
        idx_t temp = k * (k - 1);                              /* k*(k-1) */
        seq_t t = (seq_t)1 + (seq_t)(((idx_t)9 * temp) / 2);   /* A060544(k) */
        val = seq_mul(val, (seq_t)6);                          /* multiply by 6 */
        val = seq_mul(val, t);                                 /* multiply by A060544(k) */
    }
    return val;
}
