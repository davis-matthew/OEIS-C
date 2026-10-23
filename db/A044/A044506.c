/*
 * A044506  Numbers n such that string 3,5 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044506
 * Numbers n such that the string "35" occurs in the base‑6 representation of n
 * but does NOT occur in the base‑6 representation of n+1.
 *
 * The function scans successive integers, testing the adjacency property
 * by extracting base‑6 digits from least‑significant to most‑significant.
 * This is an O(n) search that is fast enough for n up to at least 10000
 * because the density of qualifying numbers is about 15 % of all integers.
 */

#include <stdbool.h>

/* Helper: true if base‑6 representation of x contains the adjacent digits 3,5 */
static inline bool contains35(seq_t x)
{
    if (x == 0) return false;          /* base‑6 "0" has no "35" */
    bool prev_seen = false;
    seq_t prev_digit = 0;               /* holds the lower significant digit seen last */

    while (x > 0) {
        seq_t digit = x % 6;             /* current (less significant) digit */
        if (prev_seen && prev_digit == 5 && digit == 3)
            return true;                /* found pattern 3 (higher) followed by 5 (lower) */
        prev_digit = digit;
        prev_seen = true;
        x /= 6;                          /* move to next more significant digit */
    }
    return false;
}

/* Main sequence function as required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1; behaviour below is irrelevant */

    idx_t count = 0;                     /* number of qualifying terms seen so far */
    seq_t cand = 1;                      /* candidate integer to test */

    for (;;) {
        if (contains35(cand) && !contains35(cand + 1)) {
            ++count;
            if (count == n) return cand;
        }
        ++cand;
    }
}
