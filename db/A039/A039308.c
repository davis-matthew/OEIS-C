/*
 * A039308  Numbers whose base-7 representation has the same nonzero number of
 *          0's and 1's.
 * https://oeis.org/A039308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Enumerates integers whose base‑7 representation contains a
 *            non‑zero equal number of 0’s and 1’s
 * Range:     u8 n <= 19, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 8459 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: return true iff x has the same non‑zero number of 0‑digits and 1‑digits in base 7. */
static bool has_equal_zero_one(seq_t x)
{
    unsigned zeros = 0, ones = 0;
    if (x == 0) return false;
    while (x > 0) {
        unsigned digit = (unsigned)(x % 7);
        if (digit == 0) ++zeros;
        else if (digit == 1) ++ones;
        x /= 7;
    }
    return zeros > 0 && zeros == ones;
}

/* Return the n-th term of sequence A039308 (1‑based). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t num   = 1;
    for (;;) {
        if (has_equal_zero_one(num)) {
            ++count;
            if (count == n) return num;
        }
        ++num;
    }
}
