/*
 * A044547  Numbers n such that string 4,0 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function returns the n-th integer m such that the base‑7 representation
 * of m contains the substring "40" while the base‑7 representation of m+1 does
 * not contain that substring.  The definition is implemented by a simple
 * brute‑force scan.
 */

#include <stdbool.h>

static inline bool has_40_in_base7(seq_t x)
{
    /* Scan the digits of x from least significant to most significant.  */
    seq_t prev = 8;                  /* sentinel digit not in [0..6] */
    while (x > 0) {
        seq_t d = x % 7;
        /* In LSB-first order we see the right digit of the pair first,
           so detect 0 (prev) followed by 4 (current). */
        if (prev == 0 && d == 4)
            return true;
        prev = d;
        x /= 7;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0; /* how many qualifying numbers have been found */
    seq_t value = 0; /* current candidate number */

    while (count < n) {
        ++value; /* move to the next integer */

        /* Check the condition for value and value+1 */
        if (has_40_in_base7(value) && !has_40_in_base7(value + 1))
            ++count;
    }

    return value; /* value is the n-th qualifying integer */
}
