/*
 * A044655  Numbers n such that string 2,5 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 890, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044655: numbers n such that the base‑9 representation of n
 * contains the substring "25" while that of n+1 does not. */

#include <stdbool.h>

/* Test whether the base‑9 representation of x contains the
 * adjacent digits 2 followed by 5. */
static bool has25(seq_t x)
{
    /* pattern 2,5 must occur with 2 as a more significant digit than 5.
       Scan digits from least‑significant to most‑significant.       */
    if (x == 0)                /* empty representation – impossible here */
        return false;

    seq_t prev = x % (seq_t)9;  /* least‑significant digit */
    x       /= (seq_t)9;

    while (x > 0) {
        seq_t d = x % (seq_t)9;
        if (d == (seq_t)2 && prev == (seq_t)5)
            return true;
        prev = d;
        x   /= (seq_t)9;
    }
    return false;
}

/* Return the n‑th term (n ≥ 1) of A044655.  The offset is 1. */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset handling – undefined for 0 */
        return 0;

    seq_t count    = (seq_t)0;
    seq_t candidate = (seq_t)1;

    while (count < n) {
        if (has25(candidate) && !has25(candidate + (seq_t)1)) {
            count++;
            if (count == n)
                return candidate;
        }
        candidate++;
    }
    /* Unreachable unless overflow, but return 0 to satisfy the compiler */
    return (seq_t)0;
}
