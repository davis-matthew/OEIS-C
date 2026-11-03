/*
 * A100449  Number of ordered pairs (i,j) with |i| + |j| <= n and gcd(i,j) <=
 *          1.
 * https://oeis.org/A100449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 4*sum_{k=1}^{n} phi(k)
 * Range:     u8 n <= 13, u16 n <= 231, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9720 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/totient.h"

/* Sum φ(k) for k = 1 .. n. */
static seq_t sum_phi_upto(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
        sum += totient(k);
    return sum;
}

/* The required prefixed function. */
seq_t a(idx_t n)
{
    seq_t s = sum_phi_upto(n);   /* Σ_{k=1}^n φ(k) */
    return 1 + (s << 2);         /* 1 + 4*sum */
}
