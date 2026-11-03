/*
 * A173430  Last of consecutive coprime iterations of sum-of-divisors
 *          function.
 * https://oeis.org/A173430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = last element in the longest prefix of the sigma iteration
 *            starting at n, where every consecutive pair of iterates is
 *            coprime. The iteration stops when a pair has gcd > 1. For n = 1,
 *            the sequence is constant 1
 * Range:     u8 n <= 49, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9657 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but avoid UB */
    if (n == 1) return 1;           /* special case */

    seq_t cur = (seq_t)n;           /* current iterate */
    while (true)
    {
        /* cast cur to idx_t for sigma_of (fits for n up to at least 10000) */
        idx_t cur_idx = (idx_t)cur;
        seq_t nxt = sigma_of(cur_idx); /* next iterate */

        /* gcd of two seq_t numbers */
        seq_t g = gcd(cur, nxt);
        if (g != 1) break;          /* stop when not coprime */

        cur = nxt;                  /* continue iteration */
    }
    return cur;
}
