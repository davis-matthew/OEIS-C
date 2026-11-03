/*
 * A044450  Numbers k such that string 2,0 occurs in the base 4 representation
 *          of k but not of k+1.
 * https://oeis.org/A044450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 953 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044450: numbers k such that the base‑4 representation of k
 * contains the substring "20" but the base‑4 representation of k+1 does
 * not. */

#include <stdbool.h>

/* Return true if x (in base 4) contains the adjacent digits 2,0
 * (with 2 more significant than 0).  Digits are examined from least-
 * significant to most‑significant; the test is therefore
 * curr==2 && prev==0. */
static bool has20_base4(seq_t x)
{
    if (x == 0) return false;

    seq_t prev = x & 3;     /* least significant digit */
    x >>= 2;
    while (x != 0) {
        seq_t curr = x & 3; /* next more significant digit */
        if (curr == 2 && prev == 0) return true;
        prev = curr;
        x >>= 2;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; generate terms until the nth one is reached. */
    idx_t count = 0;
    seq_t k = 0;

    while (count < n) {
        if (has20_base4(k) && !has20_base4(k + 1)) {
            ++count;
            if (count == n) return k;
        }
        ++k;
    }
    /* This point should never be reached. */
    return 0;
}
