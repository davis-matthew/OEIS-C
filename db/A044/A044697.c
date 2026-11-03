/*
 * A044697  Numbers n such that string 7,2 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044697
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

/* Sequence A044697 – Numbers n such that the base‑9 representation of n
   contains the substring "72" (digit 7 followed by 2) but the base‑9
   representation of n+1 does not.  The function simply scans
   successive integers, counts the qualifying numbers and returns the nth
   one.  This is O(T) where T≈81·n for the n-th term.  */

#include <stdint.h>
#include <stdbool.h>

/* Test whether the decimal digits of x in base 9 contain the pair 7,2 */
static bool contains_72(idx_t x)
{
    if (x == 0) return false;
    int prev = -1;          /* previous (more significant) digit */
    while (x > 0) {
        int d = x % 9;
        if (prev == 2 && d == 7) return true;  /* 7 followed by 2 */
        prev = d;
        x /= 9;
    }
    return false;
}

/*  Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for n=0, return 0 */

    idx_t count = 0;
    idx_t cand  = 1;                /* start testing from 1 */

    for (;;) {
        if (contains_72(cand) && !contains_72(cand + 1)) {
            ++count;
            if (count == n) return (seq_t)cand;
        }
        ++cand;
    }
}
