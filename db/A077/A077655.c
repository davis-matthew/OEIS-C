/*
 * A077655  Number of consecutive successors of n having the same number of
 *          prime factors as n (counted with multiplicity).
 * https://oeis.org/A077655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{k ≥ 0 | Ω(n+i) = Ω(n) for all 1 ≤ i ≤ k} * where Ω(n)
 *            is the number of prime factors of n counted with multiplicity *
 *            (Ω(1) = 0). This function computes a(n) by testing consecutive *
 *            integers n+1, n+2, … until the Ω value differs, counting how
 *            many * successive integers share the same...
 * Range:     u8 n <= 9468, u16 n <= 9468, u32 n <= 9468, u64 n <= 9468,
 *            u128 n <= 9468, big any n
 * Helpers:   big_omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for standard integer types */
#include "../utils/big_omega.h"

seq_t a(idx_t n)
{
    seq_t omega_n = big_omega(n);
    for (idx_t k = 1; ; ++k)
    {
        if (big_omega(n + k) != omega_n)
            return (seq_t)(k - 1);
    }
}
