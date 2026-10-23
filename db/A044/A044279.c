/*
 * A044279  Numbers k such that string 3,1 occurs in the base 9 representation
 *          of k but not of k-1.
 * https://oeis.org/A044279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 890, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A044279
 * Strings k such that base‑9 representation of k contains the digit pair 3,1
 * but that of k−1 does not.  The first term (offset = 1) is 28.
 *
 * This implementation scans integers incrementally, testing each candidate
 * until the requested nth term is found.  It uses only integer arithmetic
 * and a small buffer for the base‑9 digits of a number, so it works for
 * any seq_t size that can hold the returned value.
 */

#include <stdbool.h>

/* Return true if the base‑9 representation of x contains the adjacent
 * digits 3 followed by 1.  The digits are examined from most to least
 * significant. */
static bool contains31(seq_t x)
{
    /* A number less than 9^64 fits 64 base‑9 digits –
     * enough for all terms in typical tests (≈10^6). */
    char buf[64];
    int len = 0;

    /* Handle zero explicitly to avoid an empty buffer. */
    if (x == 0) return false;

    /* Extract digits in reverse order. */
    seq_t d = x;
    while (d > 0) {
        buf[len++] = (char)(d % 9);
        d /= 9;
    }

    /* Scan from most‑significant to least‑significant. */
    for (int i = len - 1; i > 0; --i) {
        if (buf[i] == 3 && buf[i - 1] == 1)
            return true;
    }
    return false;
}

/* Return the n-th term of the sequence, where n≥1. */
seq_t a(idx_t n)
{
    seq_t k = 1;          /* candidate number */
    idx_t found = 0;      /* how many valid numbers seen */

    while (1) {
        if (contains31(k) && !contains31(k - 1)) {
            ++found;
            if (found == n)
                return k;
        }
        ++k;
    }
}
