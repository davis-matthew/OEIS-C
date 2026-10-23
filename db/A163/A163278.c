/*
 * A163278  Concave-convex numbers.
 * https://oeis.org/A163278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Concave-convex numbers for OEIS A163278.
   A(n) is the n-th positive integer > 999 whose decimal digits
   contain a difference sequence that is neither nondecreasing
   (convex) nor nonincreasing (concave).  Such numbers are also not
   straight‑line (constant difference).  The sequence starts
   1010,1011,1020,1021,… as given in the OEIS entry.
*/
#include <stdbool.h>
#include <stddef.h>

/* Return true if the decimal digit difference sequence of n is
   neither nondecreasing nor nonincreasing.  Numbers with length < 4
   are not considered. */
static bool is_allowed(seq_t n) {
    /* Extract digits (least significant first). */
    unsigned char digs[32];   /* enough for any 128‑bit number */
    int len = 0;
    seq_t tmp = n;
    while (tmp > 0) {
        digs[len++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }
    if (len < 4) return false;   /* fewer than 4 digits */

    /* Compute differences in the order of the digits (most → least).
       We iterate from the most significant digit downwards. */
    int prev_digit = digs[len - 1];
    int prev_diff  = 0;          /* dummy initialize */
    bool first_diff = true;
    bool nondecreasing = true;
    bool nonincreasing = true;

    for (int idx = len - 2; idx >= 0; --idx) {
        int cur_digit = digs[idx];
        int diff = (int)cur_digit - (int)prev_digit;

        if (first_diff) {
            prev_diff   = diff;
            first_diff  = false;
        } else {
            if (diff < prev_diff) nondecreasing = false;
            if (diff > prev_diff) nonincreasing = false;
            prev_diff = diff;
        }
        prev_digit = cur_digit;
    }

    /* Exclude if the sequence is convex (nondecreasing)
       or concave (nonincreasing). */
    return !(nondecreasing || nonincreasing);
}

seq_t a(idx_t n) {
    idx_t count = 0;
    for (seq_t cur = 1000; ; ++cur) {      /* start at the first number with 4 digits */
        if (is_allowed(cur)) {
            ++count;
            if (count == n) return cur;
        }
    }
}
