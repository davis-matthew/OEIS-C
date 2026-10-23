/*
 * A044470  Numbers n such that string 2,2 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044470:
   a(n) = the nth integer n (1‑based) whose base‑5 representation
   contains the substring "22" but whose successor n+1 does not. */
#include <stdbool.h>

static bool has22(seq_t x)
{
    seq_t prev = (seq_t)5;                     /* sentinel > 4 */
    while (x > (seq_t)0) {
        const seq_t d = x % (seq_t)5;
        if (prev == (seq_t)2 && d == (seq_t)2)
            return true;
        prev = d;
        x = x / (seq_t)5;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* 0 is not defined; return 0 safely */
        return (seq_t)0;

    idx_t found = (idx_t)0;
    idx_t cur   = (idx_t)0;

    /* Scan successive integers until we have found the nth term. */
    while (found < n) {
        ++cur;
        if (has22((seq_t)cur) && !has22((seq_t)(cur + (idx_t)1)))
            ++found;
    }
    return (seq_t)cur;
}
