/*
 * A039058  Numbers whose base-8 representation has the same number of 1's and
 *          4's.
 * https://oeis.org/A039058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search: a(n) is the nth non‑negative integer whose
   base‑8 representation contains an equal number of 1's and 4's. */

#include <stdbool.h>

/* Return true if the base‑8 representation of x has the same count of
   digit 1's and 4's. */
static bool equal_ones_fours(seq_t x)
{
    unsigned ones = 0, fours = 0;
    /* Zero has 0 ones and 0 fours, so the test is true. */
    if (x == 0)
        return true;

    while (x > 0) {
        seq_t digit = x & 7;          /* x % 8 */
        if (digit == 1)
            ++ones;
        else if (digit == 4)
            ++fours;
        x >>= 3;                      /* x /= 8 */
    }
    return ones == fours;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1 */

    seq_t val = 0;
    idx_t count = 0;
    for (;;) {
        if (equal_ones_fours(val)) {
            ++count;
            if (count == n)
                return val;
        }
        ++val;
    }
}
