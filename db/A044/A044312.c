/*
 * A044312  Numbers n such that string 6,7 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth natural number such that its base‑9
 *            representation contains the consecutive digits 6,7, while
 *            (a(n)-1) does not
 * Range:     u8 n <= 3, u16 n <= 890, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 969 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* return true iff unsigned integer x has the substring "67" in its
   base‑9 representation (i.e. a 6 digit followed by a 7 digit). */
static bool has67_idx(idx_t x)
{
    if (x == 0) return false;
    /* process digits from least significant to most significant */
    idx_t prev = x % 9;     /* least significant digit */
    x /= 9;
    /* if number has only one digit, it cannot contain "67" */
    if (x == 0) return false;

    while (x > 0) {
        idx_t d = x % 9;   /* next more significant digit */
        x /= 9;
        if (d == 6 && prev == 7) return true;
        prev = d;
    }
    return false;
}

/* OEIS A044312: numbers n such that base‑9 representation of n
   contains "67" but base‑9 representation of n-1 does not. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1; this value is unused */

    idx_t found = 0;
    for (idx_t num = 1;; ++num) {
        if (has67_idx(num) && !has67_idx(num - 1)) {
            ++found;
            if (found == n) return (seq_t)num;
        }
    }
}
