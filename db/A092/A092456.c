/*
 * A092456  Numbers that either contain the digit 8 or are divisible by 8.
 * https://oeis.org/A092456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 950 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A092456 – Numbers that either contain the digit 8 or are divisible by 8.
   The value a(n) is obtained by scanning successive integers from 0 upward and
   selecting those that satisfy the predicate.  The function returns the nth such
   integer, where a(0) = 0.  The algorithm is purely iterative and uses only
   the integer operations permitted in the BOINC environment. */
#include <stdbool.h>

/* Return true if x contains the digit 8.  Operates on arbitrary‑precision
   seq_t by repeated division by 10. */
static bool contains_digit8(seq_t x)
{
    while (x > 0) {
        if ((x % 10) == 8)
            return true;
        x /= 10;
    }
    return false;
}

/* a(n) – n‑th element of A092456.  Linear scan from 0 upward. */
seq_t a(idx_t n)
{
    idx_t   count = 0;       /* number of qualifying terms found so far */
    seq_t   val   = 0;       /* current integer being tested */

    for (;;) {
        if ((val % 8) == 0 || contains_digit8(val)) {
            if (count == n)
                return val;
            ++count;
        }
        ++val;
    }
}
