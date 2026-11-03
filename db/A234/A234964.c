/*
 * A234964  Numbers of the form 123...n - (n+1).
 * https://oeis.org/A234964
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(1,2,…,n) – (n+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 18 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/num_digits_idx.h"

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
    if (n < 2)
        return 0;                /* offset not defined for n < 2 */

    seq_t value = 0;

    for (idx_t k = 1; k <= n; ++k) {
        int d = num_digits_idx(k);                     /* decimal digit count of k */
        value = value * pow10((idx_t)d) + (seq_t)k;
    }

    return value - ((seq_t)n + 1);
}
