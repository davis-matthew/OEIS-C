/*
 * A135873  Multiply the positive integers which are coprime to n in order
 *          (starting at 1). a(n) is the largest such partial product that is
 *          <= n.
 * https://oeis.org/A135873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest product of successive positive integers
 *            coprime to n that does not exceed n. This is obtained by
 *            multiplying the coprime integers 1,2,3,… in order until the next
 *            multiplication would exceed n; the partial product before that
 *            point is a(n)
 * Range:     u8 n <= 269, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for idx_t (uint64_t).  Allowed operations: + - * / % and comparisons. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t prod = 1;
    seq_t n_seq = (seq_t)n;

    for (idx_t k = 2;; ++k) {
        if (gcd_idx(k, n) != 1) continue;          /* skip non‑coprime k */
        seq_t k_seq = (seq_t)k;
        if (prod > n_seq / k_seq) break;           /* would exceed n */
        prod *= k_seq;
    }
    return prod;
}
