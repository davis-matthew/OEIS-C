/*
 * A039545  Numbers whose base-12 representation has the same nonzero number
 *          of 6's and 8's.
 * https://oeis.org/A039545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: Scan integers in increasing order and select those whose
 * base‑12 representation contains the same non‑zero number of 6's and 8's.  */
/* The function returns the nth such number. */
#include <stdint.h>
#include <stdbool.h>

static bool base12_has_equal_nonzero_6_and_8(seq_t x)
{
    unsigned int c6 = 0, c8 = 0;
    const seq_t base = (seq_t)12;
    while (x > (seq_t)0) {
        seq_t d = x % base;
        if (d == (seq_t)6) c6++;
        else if (d == (seq_t)8) c8++;
        x /= base;
    }
    return (c6 == c8) && (c6 > 0);
}

seq_t a(idx_t n)
{
    /* n is 1‑based */
    seq_t cur = (seq_t)80;           /* first candidate with one 6 and one 8 */
    idx_t seen = 0;
    while (seen < n) {
        if (base12_has_equal_nonzero_6_and_8(cur)) {
            seen++;
            if (seen == n) return cur;
        }
        cur++;
    }
    /* unreachable */
    return (seq_t)0;
}
