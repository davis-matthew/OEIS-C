/*
 * A039478  Numbers whose base-11 representation has the same nonzero number
 *          of 5's and 6's.
 * https://oeis.org/A039478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039478: Numbers whose base‑11 representation has
 * the same non‑zero number of 5's and 6's.
 * The function simply scans integers from 1 upward until the
 * requested term is found.  This works for any seq_t type that
 * supports the usual arithmetic and comparison operators. */

#include <stdbool.h>   /* for bool in C */

static bool has_equal_56(seq_t x)
{
    unsigned c5 = 0, c6 = 0;
    seq_t tmp = x;
    while (tmp) {
        seq_t d = tmp % (seq_t)11;
        if (d == (seq_t)5)
            ++c5;
        else if (d == (seq_t)6)
            ++c6;
        tmp /= (seq_t)11;
    }
    return (c5 == c6) && (c5 > 0);
}

seq_t a(idx_t n)
{
    /* Find the nth term by brute‑force search. */
    seq_t cur = 1;      /* candidate to test */
    idx_t count = 0;    /* number of found terms so far */

    while (count < n) {
        if (has_equal_56(cur))
            ++count;
        ++cur;
    }
    return cur - 1;     /* nth term found */
}
