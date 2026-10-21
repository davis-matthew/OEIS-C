/*
 * A082048  a(n) = least number greater than n having greater smallest prime
 *          factor than that of n.
 * https://oeis.org/A082048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 250, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: least m > n such that the smallest prime factor of m is
   greater than that of n (OEIS A082048). */
#include <stdbool.h>
#include "../utils/spf.h"

seq_t a(idx_t n)
{
    /* For n == 1 the first term is 2; the loop below handles it as well. */
    idx_t spf_n = spf(n);
    for (idx_t m = n + 1; ; ++m)
        if (spf(m) > spf_n) return (seq_t)m;
}
