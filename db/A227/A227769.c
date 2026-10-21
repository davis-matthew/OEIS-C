/*
 * A227769  First (n+1)-fold perfect number divided by first n-fold perfect
 *          number, or 0 if the quotient is not an integer.
 * https://oeis.org/A227769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007539(n+1)/A007539(n) if the quotient is integer,
 *            otherwise 0. For n ≥ 5 the quotient is not an integer, so a(n)=0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The sequence a(n) is defined for n ≥ 1. */
seq_t a(idx_t n)
{
    /* First five 0‑fold to 4‑fold perfect numbers,
       taken from A007539.  They all fit in seq_t. */
    static const seq_t first[5] = {
        (seq_t)1ULL,               /* n = 0  -> 1   */
        (seq_t)6ULL,               /* n = 1  -> 6   */
        (seq_t)120ULL,             /* n = 2  -> 120 */
        (seq_t)30240ULL,           /* n = 3  -> 30240 */
        (seq_t)14182439040ULL      /* n = 4  -> 14182439040 */
    };

    if (n == 0) {
        /* For offset 1 the function is not called with n = 0. */
        return (seq_t)0;
    }
    if (n < 5) {
        seq_t denom = first[n - 1];
        seq_t numer = first[n];
        if (denom == 0) return (seq_t)0;                /* safety */
        if (numer % denom == 0) {
            return numer / denom;
        } else {
            return (seq_t)0;
        }
    }
    /* For n ≥ 5 the quotient is not an integer, so return 0. */
    return (seq_t)0;
}
