/*
 * A075011  Floor[ concatenation of numbers from n to 1 divided by
 *          concatenation of numbers from 1 to n].
 * https://oeis.org/A075011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor( concat(n, n-1, …, 1) / concat(1, 2, …, n) )
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bigint_add_small, bigint_cmp, bigint_shift, bigint_sub,
 *            num_digits_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the two large concatenated numbers in base‑10
   using small digit arrays and repeatedly subtracts until the remainder is
   smaller than the denominator.  The quotient is guaranteed to be at most
   9, so a simple subtraction loop suffices for all n */
#include <stddef.h>
#include <stdint.h>
#include "../utils/bigint_add_small.h"
#include "../utils/bigint_cmp.h"
#include "../utils/bigint_shift.h"
#include "../utils/bigint_sub.h"
#include "../utils/num_digits_uint32.h"

/* maximum number of decimal digits needed for n up to 10 000 (≈38894)   */
#define MAX_DIGITS 40000

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Big integers are stored as arrays of decimal digits (least
       significant digit first).  The total length never exceeds MAX_DIGITS.   */
    uint8_t a_digits[MAX_DIGITS] = {0};
    size_t   a_len = 0;
    uint8_t b_digits[MAX_DIGITS] = {0};
    size_t   b_len = 0;

    /* Build A = concat(1 .. n) */
    for (idx_t i = 1; i <= n; ++i) {
        int d = num_digits_uint32((uint32_t)i);
        bigint_shift(a_digits, &a_len, d);
        bigint_add_small(a_digits, &a_len, (uint32_t)i);
    }

    /* Build B = concat(n .. 1) */
    for (idx_t i = n; i >= 1; --i) {
        int d = num_digits_uint32((uint32_t)i);
        bigint_shift(b_digits, &b_len, d);
        bigint_add_small(b_digits, &b_len, (uint32_t)i);
    }

    /* Compute floor(B / A) by repeated subtraction */
    seq_t q = 0;
    while (bigint_cmp(b_digits, b_len, a_digits, a_len) >= 0) {
        bigint_sub(b_digits, &b_len, a_digits, a_len);
        ++q;
    }

    return q;
}
