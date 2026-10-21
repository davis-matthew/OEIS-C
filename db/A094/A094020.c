/*
 * A094020  Least k <= n such that n*bigomega(k) = k*bigomega(n), where
 *          bigomega(n) = the number of prime divisors of n (counted with
 *          multiplicity) A001222.
 * https://oeis.org/A094020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = least k ≤ n such that n·Ω(k) = k·Ω(n) where Ω(i) =
 *            bigomega(i) is the number of prime factors of i (counted with
 *            multiplicity; Ω(1) = 0)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/bigomega.h"

/* Compute and return A094020(a(n)). */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n = 0, but safe for callers */
        return (seq_t)0;

    seq_t omega_n = bigomega(n);          /* Ω(n)   */
    /* Search from 1 upwards for the smallest k satisfying the condition. */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t omega_k = bigomega(k);      /* Ω(k)   */
        if ((seq_t)n * omega_k == (seq_t)k * omega_n)
            return (seq_t)k;
    }
    /* In theory we should always find a solution in the loop above
       (k = n is always a trivial solution).  The following return
       is only to satisfy the compiler about a control path that
       appears unreachable. */
    return (seq_t)n;
}
