/*
 * A044562  Numbers n such that string 6,1 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A044562
 * Definition: a(n) is the n-th positive integer whose base‑7 representation
 * contains the substring "61", but the base‑7 representation of a(n)+1 does not.
 * The function implements a straightforward search by testing successive integers.
 * It is compatible with both C11 and C++17.
 */

#include <stdbool.h>

/* Helper: check whether a base‑7 representation of x contains "61". */
static bool has61(seq_t x)
{
    if (x == (seq_t)0)
        return false;

    /* We scan the digits from least significant to most significant. */
    int previous = -1;                     /* value of the lower digit that has
                                             * already been processed */
    while (x > (seq_t)0) {
        int d = (int)(x % (seq_t)7);      /* current digit (more significant) */
        if (previous == 1 && d == 6)      /* pattern 6,1 found */
            return true;
        previous = d;
        x = x / (seq_t)7;
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t curr   = (seq_t)1;             /* start scanning from 1 */

    while (found < n) {
        if (has61(curr)) {                /* contains "61" */
            seq_t next = curr + (seq_t)1; /* successor */
            if (!has61(next)) {          /* successor does NOT contain "61" */
                ++found;
                if (found == n)
                    return curr;
            }
        }
        curr = curr + (seq_t)1;           /* move to next integer */
    }

    /* The loop always returns; this line is only to satisfy the compiler. */
    return (seq_t)0;
}
