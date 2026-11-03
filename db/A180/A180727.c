/*
 * A180727  Smallest power of 21 that begins with n.
 * https://oeis.org/A180727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = min{21^k : the decimal representation of 21^k begins with
 *            the decimal representation of n}
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static inline seq_t seq_one(void) { return (seq_t)1; }

seq_t a(idx_t n)
{
    /* Convert n to seq_t once, and compute its digit length */
    seq_t nseq = (seq_t)n;
    int len_n   = num_digits(nseq);

    /* Start with 21^0 = 1 */
    seq_t val = seq_one();

    /* Iterate over increasing exponents until a match is found */
    while (1) {
        int digits_val = num_digits(val);
        if (digits_val >= len_n) {
            /* Strip the trailing (digits_val - len_n) digits */
            seq_t leading = val / pow10(digits_val - len_n);
            if (leading == nseq)
                return val;
        }
        /* Next power of 21 */
        val = val * (seq_t)21;
    }
}
