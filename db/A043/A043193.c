/*
 * A043193  Numbers k such that 1 and 7 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043193
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

/* Sequence A043193:
   a(n) = the nth integer k ≥ 1 such that in the base‑9 representation
   of k the digits 1 and 7 appear consecutively (either "17" or "71"),
   while the base‑9 representation of k − 1 does not contain such
   a pair. */
#include <stdbool.h>

/* Test whether x's base‑9 representation contains the digits 1 and 7
   side‑by‑side in either order. */
static bool has_17_or_71(seq_t x)
{
    if (x == (seq_t)0) return false;   /* 0 has digits "0" only */

    /* digits[0] = least‑significant digit, digits[len‑1] = most‑significant */
    unsigned int digits[128];
    int len = 0;
    seq_t t = x;
    while (t > (seq_t)0) {
        digits[len++] = (unsigned int)(t % (seq_t)9);
        t /= (seq_t)9;
    }

    /* Looking for 17 or 71: digits[i] more significant than digits[i‑1] */
    for (int i = 1; i < len; ++i) {
        unsigned int d = digits[i];
        unsigned int d_prev = digits[i - 1];
        if ((d == 1 && d_prev == 7) || (d == 7 && d_prev == 1))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    seq_t k = 1;            /* candidate k */
    idx_t count = 0;        /* number of qualifying k's found */

    for (;;) {
        if (has_17_or_71(k)) {
            seq_t km1 = k - (seq_t)1;
            if (!has_17_or_71(km1)) {
                ++count;
                if (count == n)
                    return k;
            }
        }
        ++k;
    }
}
