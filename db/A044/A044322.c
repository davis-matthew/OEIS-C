/*
 * A044322  Numbers n such that the string 7,8 occurs in the base 9
 *          representation of n but not of n-1.
 * https://oeis.org/A044322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 71 + 81 * k, where k is the n‑th nonnegative integer
 *            whose base‑9 representation contains no "78" substring
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Check if the base‑9 representation of x contains the substring 7 followed by 8. */
static bool has78_base9(seq_t x)
{
    if (x < (seq_t)9) /* single digit: cannot contain "78" */
        return false;
    seq_t prev = x % (seq_t)9;   /* least significant digit */
    x /= (seq_t)9;
    while (x > (seq_t)0) {
        seq_t d = x % (seq_t)9;     /* next more significant digit */
        if (prev == (seq_t)8 && d == (seq_t)7)
            return true;
        prev = d;
        x /= (seq_t)9;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) { /* undefined for offset 1, but guard anyway */
        return (seq_t)0;
    }

    seq_t k = (seq_t)0;     /* candidate prefix */
    idx_t seen = 0;         /* how many valid prefixes we've found */

    while (seen < n) {
        if (!has78_base9(k)) {
            seen++;
            if (seen == n) {
                /* a(n) = 71 + 81 * k */
                return (seq_t)71 + (seq_t)81 * k;
            }
        }
        ++k;
    }
    /* Should never reach here for n >= 1 */
    return (seq_t)0;
}
