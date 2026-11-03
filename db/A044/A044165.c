/*
 * A044165  Numbers k such that string 3,6 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer k such that the base‑7 representation of
 *            k contains the substring "36" but the base‑7 representation of
 *            k-1 does not
 * Range:     u8 n <= 6, u16 n <= 1485, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9267 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Does x have the consecutive digits 3,6 in its base‑7 representation? */
static bool has36(idx_t x)
{
    /* Scan from least significant digit to most.  The pattern
       "36" in the usual left‑to‑right string occurs when the
       high‑order digit is 3 and the next lower is 6; during the
       reverse scan this shows as 3 after a preceding 6. */
    idx_t prev = 7;                 /* sentinel that is not a base‑7 digit */
    while (x > 0) {
        idx_t d = x % 7;
        if (d == 3 && prev == 6)
            return true;           /* found "36" */
        prev = d;
        x /= 7;
    }
    return false;
}

/* Return the n‑th term of the sequence.  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined for n == 0, but guard anyway */
        return 0;

    idx_t hits = 0;                 /* number of qualifying integers found */
    idx_t k    = 0;                 /* current integer being tested */

    while (hits < n) {
        ++k;
        if (has36(k) && !has36(k - 1))
            ++hits;                 /* found a qualifying integer */
    }
    return (seq_t)k;                /* cast to the sequence's integer type */
}
