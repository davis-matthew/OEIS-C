/*
 * A044639  Numbers n such that string 0,7 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044639:   Numbers n such that the base‑9 representation of n contains
 *            the substring "07" but the base‑9 representation of n+1 does not.
 *
 * The function scans integers starting from 1, counting those that satisfy
 * the condition until the nth such integer is found.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static inline bool has07(seq_t v) {
    /* Scan the base‑9 digits from least significant to most significant. */
    seq_t prev = 0;
    bool first = true;     /* skip the first digit pair */
    while (v > 0) {
        seq_t d = v % 9;
        if (!first && d == 0 && prev == 7)
            return true;
        prev = d;
        first = false;
        v /= 9;
    }
    return false;
}

seq_t a(idx_t n) {
    /* Offset is 1; return 0 for n==0 (behavior below offset is irrelevant). */
    if (n == 0)
        return (seq_t)0;

    idx_t count = 0;     /* how many qualifying numbers have been seen */
    seq_t candidate = 1; /* number to test */

    while (count < n) {
        if (has07(candidate) && !has07(candidate + (seq_t)1)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }

    /* This point is unreachable because the loop always returns. */
    return (seq_t)0;
}
