/*
 * A100449  Number of ordered pairs (i,j) with |i| + |j| <= n and gcd(i,j) <=
 *          1.
 * https://oeis.org/A100449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 4*sum_{k=1}^{n} phi(k)
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   phi_u64, sum_phi_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/phi_u64.h"
#include "../utils/sum_phi_upto.h"

/* The required prefixed function. */
seq_t a(idx_t n)
{
    seq_t s = sum_phi_upto(n);   /* Σ_{k=1}^n φ(k) */
    return 1 + (s << 2);         /* 1 + 4*sum */
}
