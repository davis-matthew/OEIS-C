/*
 * A032903  Numbers whose base-6 representation Sum_{i=0..m} d(i)*6^i has d(0)
 *          <= d(1) >= d(2) <= ...
 * https://oeis.org/A032903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A032903:
 *
 * A032903: Numbers whose base‑6 representation satisfies
 *          d0 ≤ d1 ≥ d2 ≤ d3 ≥ …
 *
 * This implementation simply scans integers in increasing order
 * and counts those that respect the alternating inequality.
 * Once the n‑th such integer is found it is returned.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: returns true if x satisfies the alternating inequalities
   in its base‑6 representation. */
static bool good(seq_t x)
{
    seq_t t = x;
    seq_t prev = 0;          /* value of the previous lower digit */
    bool first = true;       /* whether we are at the least significant digit */
    bool le = true;          /* next comparison is “≤” (since d0 ≤ d1) */

    while (t) {
        seq_t d = t % (seq_t)6;

        if (!first) {
            if (le) {
                if (prev > d) return false;   /* d_{k-1} must be ≤ d_k */
            } else {
                if (prev < d) return false;   /* d_{k-1} must be ≥ d_k */
            }
            le = !le;         /* alternate the inequality direction */
        }

        prev = d;
        first = false;
        t /= (seq_t)6;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t val = 1;

    while (count < n) {
        if (good(val)) {
            count++;
            if (count == n) {
                return val;
            }
        }
        val++;
    }
    /* Should never reach here for n >= 1 */
    return (seq_t)0;
}
