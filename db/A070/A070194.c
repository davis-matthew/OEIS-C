/*
 * A070194  List the phi(n) numbers from 1 to n-1 which are relatively prime
 *          to n; sequence gives size of maximal gap.
 * https://oeis.org/A070194
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = maximum difference between consecutive integers in [1,
 *            n-1] that are coprime to n. That is, if S = {k | 1 ≤ k < n,
 *            gcd(k,n)=1}, a(n) = max{ s_i+1 - s_i | s_i, s_i+1 ∈ S, s_i <
 *            s_i+1 } for n ≥ 3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t gcd_idx(idx_t a, idx_t b) {
    /* Euclidean algorithm for unsigned idx_t values */
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    /* a(3) is the first defined term; values for n < 3 are unused */
    if (n < 3) return 0;

    seq_t max_gap = 0;
    idx_t prev = 0;
    int first = 1;          /* flag for the first coprime found */

    for (idx_t r = 1; r < n; ++r) {
        if (gcd_idx(r, n) == 1) {        /* r is coprime to n */
            if (first) {
                prev = r;
                first = 0;
            } else {
                seq_t diff = (seq_t)r - (seq_t)prev;
                if (diff > max_gap) max_gap = diff;
                prev = r;
            }
        }
    }
    return max_gap;
}
