/*
 * A126535  Number of base 14 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A126535: Number of base‑14 n‑digit numbers with adjacent digits differing by five or less.
 *
 * A(n) counts length‑n strings over the alphabet {0,…,13} such that every adjacent pair
 * satisfies |x[i] – x[i+1]| ≤ 5.  The following recurrence is used:
 *   f(1, d) = 1                     for all d = 0…13
 *   f(n, d) = Σ_{k: |k-d| ≤ 5} f(n-1, k)   for n ≥ 2
 *   A(n)   = Σ_{d=0}^{13} f(n, d)
 * (A(0) is defined to be 1, representing the empty string.)
 *
 * This implementation computes the values iteratively in O(n·14) time.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return (seq_t)1;
    }
    if (n == (idx_t)1) {
        return (seq_t)14;
    }

    /* sequence of length-1 counts: all digits are allowed */
    seq_t prev[14];
    seq_t cur[14];
    for (int d = 0; d < 14; ++d) {
        prev[d] = (seq_t)1;
    }

    /* iterate from length 2 up to n */
    for (idx_t len = 2; len <= n; ++len) {
        for (int d = 0; d < 14; ++d) {
            seq_t sum = (seq_t)0;
            int lo = d - 5;
            if (lo < 0) lo = 0;
            int hi = d + 5;
            if (hi > 13) hi = 13;
            for (int k = lo; k <= hi; ++k) {
                sum += prev[k];
            }
            cur[d] = sum;
        }
        /* copy cur to prev for next iteration */
        for (int d = 0; d < 14; ++d) {
            prev[d] = cur[d];
        }
    }

    /* total number of strings of length n */
    seq_t total = (seq_t)0;
    for (int d = 0; d < 14; ++d) {
        total += prev[d];
    }
    return total;
}
