/*
 * A099305  Number of solutions of the equation (n+k)' = n' + k', with 1 <= k
 *          <= 2n, where n' denotes the arithmetic derivative of n.
 * https://oeis.org/A099305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { k | 1 ≤ k ≤ 2n and (n+k)' = n' + k' } where ' denotes
 *            the arithmetic derivative (n = ∏ p_i^e_i ⇒ n' = n Σ e_i/p_i).
 *            The function below computes the derivative directly by factoring
 *            each argument on the fly. For n up to about 10 000 this runs in
 *            well under two seconds
 * Range:     u8 n <= 31, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   arith_deriv_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/arith_deriv_uint64.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t dn = arith_deriv_uint64((uint64_t)n);

    for (idx_t k = 1; k <= 2 * n; ++k) {
        seq_t dk = arith_deriv_uint64((uint64_t)k);
        seq_t dp = arith_deriv_uint64((uint64_t)(n + k));
        if (dp == dn + dk)
            ++count;
    }

    return count;
}
