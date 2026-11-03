/*
 * A044107  Numbers k such that string 0,5 occurs in the base 6 representation
 *          of k but not of k-1.
 * https://oeis.org/A044107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 2034, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9791 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044107: Numbers k such that base‑6 representation of k
   contains the adjacent digits 0,5 (0 more significant than 5) but
   base‑6 representation of k‑1 does not.  The function returns the nth
   such integer.  It searches integers in order, testing each for the
   required pattern. */

#include <stdbool.h>

static bool has_05_uint64(uint64_t x)
{
    /* Return true if x (in base‑6) contains the adjacent digits 0,5
       with 0 more significant than 5 (the digit 5 is the least‑significant
       of the pair). */
    if (x < 6) return false;            /* fewer than two digits */
    uint8_t prev = x % 6;                /* least significant digit */
    x      /= 6;
    while (x > 0) {
        uint8_t cur = x % 6;
        if (prev == 5 && cur == 0)
            return true;
        prev = cur;
        x    /= 6;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;

    while (count < n) {
        ++k;
        if (has_05_uint64((uint64_t)k) && !has_05_uint64((uint64_t)(k - 1))) {
            ++count;
            if (count == n)
                return (seq_t)k;   /* convert to seq_t as required */
        }
    }
    return (seq_t)0;  /* should never happen for valid n >= 1 */
}
