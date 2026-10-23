/*
 * A044093  Numbers n such that string 3,1 occurs in the base 5 representation
 *          of n but not of n-1.
 * https://oeis.org/A044093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A044093 – numbers n where the base‑5 representation
   contains the substring "31" while that of n-1 does not.
   Computed by brute‑force search. */

#include <stdbool.h>

static bool contains31(seq_t x)
{
    /* Detects whether the base-5 representation of x contains
       the adjacent digits 3 followed by 1.  The check is performed
       while scanning the digits from least significant to most
       significant, so we look for the reversed pattern 1,3. */
    seq_t prev = 0;
    bool has_prev = false;
    const seq_t five = (seq_t)5;
    while (x > 0) {
        seq_t digit = x % five;
        if (has_prev && prev == (seq_t)1 && digit == (seq_t)3)
            return true;
        prev = digit;
        has_prev = true;
        x = x / five;
    }
    return false;
}

extern seq_t a(idx_t n)
{
    /* Find the n-th integer satisfying the condition.  The search
       starts from 1 and increments until n such integers are found. */
    seq_t m = 0;
    idx_t found = 0;
    while (found < n) {
        m = m + 1;
        if (contains31(m) && !contains31(m - 1))
            ++found;
    }
    return m;
}
