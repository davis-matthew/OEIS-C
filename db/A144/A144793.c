/*
 * A144793  Consider the runs of 0's in the binary representation of n, each
 *          of these runs being on the edge of the binary representation n
 *          and/or being bounded by 1's. Consider also the runs of 1's in the
 *          binary representation of n, each of these runs being on the edge
 *          of the binary representation n and/or...
 * https://oeis.org/A144793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n-th positive integer whose shortest runs of 0's
   and shortest runs of 1's in its binary representation are equal. */
#include <stdbool.h>
#include <limits.h>

static bool equal_shortest_runs(seq_t m) {
    idx_t v = (idx_t)m;          /* m is small enough for this cast. */
    if (v == 0) return false;    /* no positive integer 0 */
    unsigned min0 = UINT_MAX;
    unsigned min1 = UINT_MAX;
    bool has0 = false, has1 = false;
    int prev = -1;
    unsigned run = 0;

    while (v > 0) {
        int bit = v & 1;
        if (prev == -1) {          /* first bit */
            run = 1;
        } else if (bit == prev) {  /* same run */
            run++;
        } else {                   /* run ended */
            if (prev == 0) {
                if (run < min0) min0 = run;
                has0 = true;
            } else {
                if (run < min1) min1 = run;
                has1 = true;
            }
            run = 1;
        }
        prev = bit;
        v >>= 1;
    }
    /* final run */
    if (prev == 0) {
        if (run < min0) min0 = run;
        has0 = true;
    } else {
        if (run < min1) min1 = run;
        has1 = true;
    }
    if (!has0 || !has1) return false;
    return min0 == min1;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* not used, but safe */
    idx_t found = 0;
    seq_t candidate = 1;
    while (found < n) {
        if (equal_shortest_runs(candidate)) {
            found++;
            if (found == n) return candidate;
        }
        candidate++;
    }
    return candidate;  /* unreachable, keeps compiler happy */
}
