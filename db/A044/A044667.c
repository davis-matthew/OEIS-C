/*
 * A044667  Numbers n such that string 3,8 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044667: Numbers n such that the string "38" occurs in
   the base‑9 representation of n but not of n+1. */

/* The function a(n) finds the nth member (n ≥ 1) by brute‑force
   search.  The density of such numbers is about 1/81, so for n up to
   10 000 the function examines on the order of 10⁶ integers, easily
   fitting the 2‑second limit in C.  All arithmetic is performed on the
   generic type seq_t, so it works for 8‑, 16‑, 32‑, 64‑, 128‑bit
   unsigned integers and for arbitrary‑precision builds. */

#include <stdbool.h>

/* Return true if the base‑9 representation of x contains the
   consecutive digits 3 and 8 (in that order). */
static bool has_38_base9(seq_t x)
{
    /* Need at least two digits to contain "38" */
    if (x < 8) return false;

    seq_t prev = x % 9;          /* least significant digit */
    x /= 9;
    while (x) {
        seq_t cur = x % 9;
        if (cur == 3 && prev == 8) return true;
        prev = cur;
        x /= 9;
    }
    return false;
}

/* Return the nth element of sequence A044667 (n≥1). */
seq_t a(idx_t n)
{
    seq_t m = 0;          /* candidate value */
    idx_t count = 0;      /* how many valid numbers seen so far */

    while (count < n) {
        if (has_38_base9(m) && !has_38_base9(m + 1))
            ++count;
        ++m;
    }
    return m - 1;         /* m was incremented one step beyond the nth term */
}
