/*
 * A090864  Complement of generalized pentagonal numbers (A001318).
 * https://oeis.org/A090864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th positive integer that is NOT a generalized
 *            pentagonal number * (complement of A001318). The implementation
 *            scans the natural numbers * and counts those that are not of the
 *            form k(3k±1)/2 for some integer k≥1
 * Range:     u8 n <= 229, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 86 terms through n = 972 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_gpn(seq_t x) {
    /* Test whether x is a generalized pentagonal number:
     * k(3k-1)/2 or k(3k+1)/2 for some integer k≥1.  */
    for (idx_t k = 1; ; ++k) {
        /* Use seq_t arithmetic to avoid overflow. */
        seq_t kk = (seq_t)k;
        seq_t g1 = (kk * (3 * kk - 1)) / 2;
        if (g1 > x) /* g1 is increasing, so we can terminate. */
            break;
        if (g1 == x)
            return true;
        seq_t g2 = (kk * (3 * kk + 1)) / 2;
        if (g2 == x)
            return true;
        /* g2 grows faster than g1, so the next iteration will also have g1 > x eventually. */
    }
    return false;
}

seq_t a(idx_t n) {
    /* Returns the n-th term (1‑based) of A090864. */
    if (n == 0)
        return (seq_t)0;  /* undefined by OEIS, but safe fallback. */

    idx_t count = 0;
    seq_t candidate = (seq_t)1;

    for (;;) {
        if (!is_gpn(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}
