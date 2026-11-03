/*
 * A044608  Numbers n such that string 5,0 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044608: returns the nth number n such that the base‑8
 * representation of n contains the substring “50” but the base‑8
 * representation of n+1 does not. */

#include <stdbool.h>
#include <stdint.h>

/* Return true if the octal representation of x contains the
 * substring "50". */
static bool has_50_oct(seq_t x)
{
    /* At most log8(x)+1 digits; for a 64‑bit integer at most 22.
     * Use a static array on the stack that is large enough
     * for any reasonable seq_t instance.  The maximum number of
     * octal digits for a 128‑bit integer is 43, for a 256‑bit
     * is 86; a few hundred slots are plenty for the test data. */
    unsigned char digs[256];
    int pos = 0;

    /* Extract digits from least‑significant to most‑significant. */
    while (x != (seq_t)0) {
        digs[pos++] = (unsigned char)(x % (seq_t)8);
        x /= (seq_t)8;
    }

    /* Scan from most‑significant to least‑significant looking for
     * the pattern 5 followed by 0. */
    for (int i = pos - 1; i > 0; --i) {
        if (digs[i] == 5 && digs[i - 1] == 0)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 for safety.  The caller normally
     * supplies n >= 1 (the OEIS offset). */
    if (n == 0) {
        return (seq_t)0;
    }

    idx_t found = 0;
    seq_t candidate = 0;

    while (found < n) {
        /* Test the conditions for the candidate. */
        if (has_50_oct(candidate) && !has_50_oct(candidate + (seq_t)1)) {
            ++found;
            if (found == n) {
                return candidate;
            }
        }
        ++candidate;
    }
    /* Unreachable for n >= 1, but keep a return to satisfy the compiler. */
    return (seq_t)0;
}
