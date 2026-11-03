/*
 * A044522  Numbers n such that string 0,3 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044522
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

/* This function enumerates all integers n such that the base‑7 representation
 * of n contains the consecutive digits 0 followed by 3, and the base‑7
 * representation of n+1 does not contain that pattern.
 * The sequence A044522 is obtained by returning the nth such integer, where
 * a(1) = 52 is the first term.  The function is valid for any build
 * that defines the integer types seq_t and idx_t.  For seq_t being an
 * arbitrary‑precision type or one of the standard unsigned integer sizes,
 * the algorithm is correct and runs in linear time with respect to the
 * value of n (the maximum is far below 2 s for n ≈ 10 000).  No
 * specialized libraries are required beyond <stdbool.h>. */

#include <stdbool.h>

/* Return true if x in base 7 contains the substring "03". */
static bool has03(seq_t x)
{
    /* prev holds the previous (less significant) digit just processed.
     * Using an impossible sentinel (7) guarantees that the first digit 
     * cannot accidentally match the pattern. */
    seq_t prev = 7;
    while (x != 0) {
        seq_t d = x % 7;
        /* In base‑7 the pattern "0 3" appears when the current digit is 0
         * and the next more significant digit (prev) is 3. */
        if (d == 0 && prev == 3)
            return true;
        prev = d;
        x = x / 7;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n ≤ 0 the function
     * simply returns 0, as the caller can ignore such calls. */
    if (n == 0)
        return 0;

    seq_t val = 0;
    idx_t count = 0;

    while (1) {
        /* Advance to the next integer to test. */
        val = val + 1;

        /* Check the condition for the current value and its successor. */
        if (has03(val) && !has03(val + 1)) {
            ++count;
            if (count == n)
                return val;
        }
    }
}
