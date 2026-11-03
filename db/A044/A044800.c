/*
 * A044800  Numbers k such that string 8,7 occurs in the base 10
 *          representation of k but not of k+1.
 * https://oeis.org/A044800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044800: Numbers k such that the decimal representation of k
   contains the substring "87" but the decimal representation of k+1 does
   not. The function generates the n‑th term by scanning successive
   integers and testing the property. */

#include <stdbool.h>

/* Test whether a decimal representation of x contains the consecutive
   digits 8 followed by 7 (i.e. the substring "87").  Scans from least
   significant digit to most significant digit. */
static bool contains_87(seq_t x)
{
    seq_t t = x;
    seq_t prev = (seq_t)0;
    while (t != (seq_t)0) {
        seq_t cur = t % (seq_t)10;
        if (prev == (seq_t)7 && cur == (seq_t)8)
            return true;
        prev = cur;
        t /= (seq_t)10;
    }
    return false;
}

/* Main sequence function.  a(1) = 87. */
seq_t a(idx_t n)
{
    idx_t found = (idx_t)0;
    seq_t k      = (seq_t)87;

    while (1) {
        bool k_has   = contains_87(k);
        seq_t next   = k + (seq_t)1;
        bool next_has = contains_87(next);

        if (k_has && !next_has) {
            ++found;
            if (found == n)
                return k;
        }
        k = next;
    }
}
