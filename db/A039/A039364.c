/*
 * A039364  Numbers whose base-9 representation has the same nonzero number of
 *          0's and 8's.
 * https://oeis.org/A039364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7535, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9795 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth term of OEIS A039364.
   The sequence consists of the positive integers whose base‑9
   representation contains the same non‑zero number of 0's and 8's.
   The algorithm simply searches forward from 1 until it has found n
   such numbers.  No external libraries are required. */

#include <stdbool.h>

/* Return true iff x (in base 9) has a non‑zero equal count of 0's
   and 8's.  The function works for any seq_t that supports %,
   /, ++, and comparison operators. */
static bool has_equal_nonzero_0_8(seq_t x)
{
    unsigned zero = 0, eight = 0;
    const seq_t nine = (seq_t)9;

    while (x > 0) {
        seq_t r = x % nine;
        unsigned d = (unsigned)r;      /* digit in range 0 … 8 */
        if (d == 0) ++zero;
        else if (d == 8) ++eight;
        x = x / nine;
    }
    return zero > 0 && eight > 0 && zero == eight;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    seq_t cand = (seq_t)1;
    idx_t found = 0;

    while (1) {
        if (has_equal_nonzero_0_8(cand)) {
            ++found;
            if (found == n) return cand;
        }
        ++cand;
    }
}
