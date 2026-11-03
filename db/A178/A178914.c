/*
 * A178914  10's complement of nonnegative numbers.
 * https://oeis.org/A178914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 10^k - n where k is the number of decimal digits in n
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 72 (2026-11-01)
 * Helpers:   num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The digit_count function may return 0 for n == 0, so we handle that
 * explicitly.  All other values are computed from the utilities. */
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
    /* Number of digits in n, treating 0 as having one digit */
    idx_t k = (n == 0) ? 1 : num_digits0(n);

    /* Compute 10^k and subtract n, all in the seq_t type */
    seq_t pow10k = pow10(k);
    return pow10k - (seq_t)n;
}
