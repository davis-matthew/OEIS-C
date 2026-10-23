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
 * Helpers:   has61
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
#include "../utils/has61.h"

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
