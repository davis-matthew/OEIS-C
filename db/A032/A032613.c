/*
 * A032613  Concatenation of n and n + 8 or {n,n+8}.
 * https://oeis.org/A032613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n*10^digits(n+8) + (n+8)
 * Range:     u8 n <= 2, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
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
    /* Compute n+8 as idx_t (no risk of overflow for idx_t type) */
    idx_t n_plus = n + (idx_t)8;

    /* Number of decimal digits of n+8 */
    int d = num_digits_idx(n_plus);

    /* 10^digits(n+8) as seq_t */
    seq_t shift = pow10((idx_t)d);

    /* Concatenate n and n+8 */
    seq_t n_seq = (seq_t)n;
    seq_t n_plus_seq = (seq_t)n_plus;
    seq_t res = n_seq * shift + n_plus_seq;

    return res;
}
