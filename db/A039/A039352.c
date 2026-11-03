/*
 * A039352  Numbers whose base-8 representation has the same nonzero number of
 *          4's and 6's.
 * https://oeis.org/A039352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A039352: Numbers whose base‑8 representation has the same
   nonzero number of 4's and 6's.
   Formula: a(n) = the nth integer k such that in base 8
   count_4(k) == count_6(k)  > 0. */

/* The implementation simply scans the positive integers and counts
   4's and 6's in octal (base 8).  The first n numbers satisfying the
   property are returned, so a(1) = 38, a(2) = 52, etc. */

#include <stdbool.h>

/* Helper that returns true iff x has the same non‑zero number
   of 4’s and 6’s in its base‑8 representation. */
static bool has_equal_nonzero_4_6(seq_t x)
{
    int cnt4 = 0;
    int cnt6 = 0;
    /* Decompose x into base‑8 digits by repeated division by 8.
       For uint*_t types this works fine; for arbitrary‑precision (C++)
       integer classes the built‑in operators `>>= 3` and `& 7` are also
       defined. */
    while (x != (seq_t)0) {
        int d = (int)(x & (seq_t)7);  /* same as x % 8 */
        if (d == 4)           ++cnt4;
        else if (d == 6)      ++cnt6;
        x >>= 3;              /* same as x /= 8 */
    }
    return (cnt4 > 0) && (cnt4 == cnt6);
}

/* Main sequence function required by the build system. */
seq_t a(idx_t n)
{
    /* n is 1‑based; behaviour for n < 1 is undefined. */
    idx_t found = 0;
    seq_t cur   = (seq_t)1;

    for (;;) {
        if (has_equal_nonzero_4_6(cur)) {
            ++found;
            if (found == n)
                return cur;
        }
        ++cur;
    }
}
