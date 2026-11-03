/*
 * A053067  a(n) is the concatenation of next n numbers (omit leading 0's).
 * https://oeis.org/A053067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = concatenation of integers from A000124(n) to A000217(n)
 *            inclusive (A000124(n) = n*(n-1)/2 + 1, A000217(n) = n*(n+1)/2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 196 (2026-11-01)
 * Helpers:   num_digits0_seq_to_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/num_digits0_seq_to_i32.h"

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
    if (n == 0) return 0;                /* safeguard for possible n==0 */

    /* compute start = A000124(n) and end = A000217(n) */
    seq_t start = ((seq_t)n * ((seq_t)n - 1) / 2) + 1;
    seq_t end   = ((seq_t)n * ((seq_t)n + 1) / 2);

    seq_t res = 0;

    /* concatenate numbers start .. end */
    for (idx_t k = 0; k < n; ++k) {
        seq_t i = start + (seq_t)k;            /* current number */
        int d   = num_digits0_seq_to_i32(i);               /* decimal digits of i   */
        seq_t mult = pow10((idx_t)d);          /* 10^d                 */
        res = res * mult + i;
    }
    return res;
}
