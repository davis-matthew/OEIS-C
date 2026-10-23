/*
 * A043967  Numbers k such that 0 and 8 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043967:
   Numbers k such that 0 and 8 occur juxtaposed in the base‑9 representation
   of k but not of k+1.  The function returns the n‑th such number (a(1) = 72). */

#include <stdbool.h>
#include <stddef.h>

/* Test whether the base‑9 representation of x contains adjacent digits 0 and 8. */
static bool has08(seq_t x)
{
    /* The representation of 0 is "0", which does not contain a 0–8 pair. */
    if (x == 0) return false;

    seq_t prev = x % 9;
    x = x / 9;
    while (x != 0) {
        seq_t cur = x % 9;
        if ((prev == 0 && cur == 8) || (prev == 8 && cur == 0))
            return true;
        prev = cur;
        x = x / 9;
    }
    return false;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */

    idx_t found = 0;
    seq_t k = 0;

    while (found < n) {
        if (has08(k)) {
            seq_t kp1 = k + 1;
            if (!has08(kp1)) {
                ++found;
                if (found == n)
                    return k;
            }
        }
        ++k;
    }
    return 0;   /* unreachable */
}
