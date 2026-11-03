/*
 * A019566  The differences 1-1, 21-12, 321-123, ..., 10987654321-12345678910,
 *          1110987654321-1234567891011, etc.
 * https://oeis.org/A019566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, easy
 * Formula:   a(n) = concatenation(n,n-1,…,1) - concatenation(1,2,…,n) (i.e. 9
 *            * A083449(n)). Computed by building the ascending and descending
 *            concatenations iteratively with powers of ten
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 10, i64 n <= 14, i128 n <= 24,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 345 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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
    /* For n==0 the offset is 1, but any value is acceptable. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Build ascending concatenation: 1 2 … n */
    seq_t asc = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t p = pow10((idx_t)num_digits_idx(i));
        asc = asc * p + (seq_t)i;
    }

    /* Build descending concatenation: n … 2 1 */
    seq_t desc = (seq_t)n;
    /* We loop down to 1.  The unsigned loop will stop when i==0. */
    for (idx_t i = n; i >= 1; --i) {
        if (i == 0) {          /* wrap‑around guard for unsigned i */
            break;
        }
        if (i == n) {
            continue;          /* already set desc = n */
        }
        seq_t p = pow10((idx_t)num_digits_idx(i));
        desc = desc * p + (seq_t)i;
    }

    /* Result is descending minus ascending */
    return desc - asc;
}
