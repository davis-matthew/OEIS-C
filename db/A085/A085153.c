/*
 * A085153  All prime factors of n and n+1 are <= 7. (Related to the abc
 *          conjecture.)
 * https://oeis.org/A085153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: full, pass, 23 terms through n = 23 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A085153: numbers n such that all prime factors of n and n+1 are ≤ 7.  */
/* The term a(n) (n ≥ 1) is the n‑th member of this subsequence of N.       */
#include <stdbool.h>

static inline bool is_smooth7(seq_t m)
{
    if (m == 0) return false;        /* 0 has no prime factorisation in this sense */
    /* Strip off all factors 2,3,5,7 */
    while ((m & 1) == 0) m >>= 1;          /* divide by 2 */
    while (m % 3 == 0) m /= 3;
    while (m % 5 == 0) m /= 5;
    while (m % 7 == 0) m /= 7;
    return m == 1;
}

seq_t a(idx_t n)
{
    idx_t found = 0;          /* how many terms found so far */
    seq_t m = 1;              /* current candidate */

    while (1) {
        if (is_smooth7(m) && is_smooth7(m + 1)) {
            ++found;
            if (found == n)
                return m;
        }
        ++m;
    }
}
