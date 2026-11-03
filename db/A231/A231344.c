/*
 * A231344  Number of morphisms in full subcategories of Set spanned by {{},
 *          {1}, {1, 2}, ..., {1, 2, ..., n}}.
 * https://oeis.org/A231344
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 345 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{0 <= i,j <= n} i^j     (0^0 = 1)
 *
 * Compute using the geometric series:
 *   a(n) = 1                                 // i = 0
 *        + (n+1)                             // i = 1
 *        + Σ_{i=2..n}  (i^{n+1} - 1) / (i-1)
 *
 * For n < -1 returns 0 (undefined); for n = -1 returns 0 (empty set).
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow.h"

seq_t a(idx_t n) {
    if (n < -1) return (seq_t)0;      /* undefined range, safe return */
    if (n == -1) return (seq_t)0;      /* sequence offset */
    seq_t result = (seq_t)0;
    idx_t i;

    /* i = 0 term: 0^0 = 1 */
    result += (seq_t)1;

    /* handle i = 1 term */
    if (n >= 1) {
        result += (seq_t)(n + 1);
    }

    /* i from 2 to n */
    for (i = 2; i <= n; ++i) {
        seq_t base = (seq_t)i;
        seq_t exp  = (seq_t)n + 1;          /* n+1 */
        seq_t power = ipow(base, exp);   /* base^(n+1) */
        seq_t term = (power - (seq_t)1) / (base - (seq_t)1);
        result += term;
    }
    return result;
}
