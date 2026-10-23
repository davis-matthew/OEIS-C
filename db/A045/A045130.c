/*
 * A045130  Numbers whose base-4 representation contains exactly four 1's and
 *          one 3.
 * https://oeis.org/A045130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose base‑4 representation
 *            contains exactly four 1’s and one 3
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Check whether a number has exactly four 1's and one 3 in its base‑4 digits. */
static inline bool is_valid(seq_t v)
{
    int cnt1 = 0, cnt3 = 0;
    while (v > 0) {
        seq_t d = v % 4;          /* base‑4 digit */
        if (d == 1) {
            if (++cnt1 > 4) return false;
        } else if (d == 3) {
            if (++cnt3 > 1) return false;
        }
        v /= 4;
    }
    return cnt1 == 4 && cnt3 == 1;
}

/* Return the n-th element of the sequence A045130. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n = 0, guard clause */

    seq_t x   = 0;   /* current candidate */
    idx_t seen = 0;  /* how many valid numbers seen so far */

    for (;; ++x) {
        if (is_valid(x)) {
            ++seen;
            if (seen == n) {
                return x;
            }
        }
    }
}
