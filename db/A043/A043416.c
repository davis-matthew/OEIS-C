/*
 * A043416  Numbers whose base-7 representation contains exactly four 5's.
 * https://oeis.org/A043416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑7 representation
 *            contains exactly four 5's. (OEIS A043416)
 * Range:     u8 none, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>    /* for standard types (if needed) */

/* Count whether a base‑7 representation of x contains exactly four 5's.
   Uses only integer arithmetic. */
static int has_four_fives_in_base7(seq_t x)
{
    int cnt = 0;
    /* Loop until x becomes zero.  For x == 0, the loop is skipped. */
    while (x != (seq_t)0) {
        /* % and / are defined for seq_t.  Compare to 5 (converted). */
        if (x % (seq_t)7 == (seq_t)5)
            ++cnt;
        if (cnt > 4)   /* quick exit if too many 5's */
            return 0;
        x /= (seq_t)7;
    }
    return cnt == 4;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* We search sequentially for the nth number with the property. */
    seq_t candidate = (seq_t)0;
    idx_t found = (idx_t)0;

    while (found < n) {
        ++candidate;                     /* move to next integer */
        if (has_four_fives_in_base7(candidate))
            ++found;                     /* successful candidate */
    }

    return candidate;
}
