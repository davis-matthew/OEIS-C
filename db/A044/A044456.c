/*
 * A044456  Numbers n such that string 3,2 occurs in the base 4 representation
 *          of n but not of n+1.
 * https://oeis.org/A044456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044456: Numbers n such that the base‑4 representation of n
   contains the consecutive digits 3,2 while the base‑4 representation of
   n+1 does not.  This function computes the n‑th such number by a
   straightforward search. */

#include <stdbool.h>

/* Test whether a base‑4 representation of a number contains the digit
   pair 3,2.  The routine works for any seq_t capable of division and
   modulo by small integers. */
static bool contains32(seq_t x)
{
    if (x == 0) return false;

    /* Store base‑4 digits in least‑significant‑first order.  The
       largest relevant number for this sequence is well below 4^64, so
       an array of 128 digits is more than sufficient. */
    unsigned char digits[128];
    idx_t len = 0;

    while (x > 0) {
        seq_t d = x % 4;
        digits[len++] = (unsigned char)d;
        x = x / 4;
    }

    /* Scan from the most‑significant digit downwards. */
    for (idx_t i = len; i-- > 1; ) {
        if (digits[i] == 3 && digits[i-1] == 2)
            return true;
    }
    return false;
}

/* Return the n‑th term of the sequence (1‑based).  For n < 1 the
   function returns 0.  The algorithm increments from the smallest
   candidate until the required count is reached. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t count = 0;
    seq_t x = 0;

    while (count < n) {
        if (contains32(x) && !contains32(x + 1))
            ++count;
        if (count < n)
            ++x;
    }
    return x;
}
