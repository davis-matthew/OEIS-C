/*
 * A043209  Numbers k such that 4 and 8 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that the base‑9 representation of k contains the
 * digits 4 and 8 adjacent (in either order) but the representation of
 * k−1 does not. This is a direct search for the n‑th such integer. */
#include <stdbool.h>

static bool has_adjacent_48(seq_t x)
{
    seq_t prev = (seq_t)10;                  /* sentinel larger than any base‑9 digit */
    while (x > (seq_t)0) {
        seq_t d = x % (seq_t)9;             /* least significant digit */
        if ((d == (seq_t)4 && prev == (seq_t)8) ||
            (d == (seq_t)8 && prev == (seq_t)4))
            return true;
        prev = d;
        x /= (seq_t)9;                      /* shift right in base‑9 */
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)                /* offset is 1; undefined for n=0 */
        return (seq_t)0;

    seq_t k = (seq_t)1;
    idx_t count = (idx_t)0;

    while (1) {
        if (has_adjacent_48(k) && !has_adjacent_48(k - (seq_t)1)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
