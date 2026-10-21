/*
 * A062173  a(n) = 2^(n-1) mod n.
 * https://oeis.org/A062173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n‑1) mod n, n ≥ 1
 * Range:     u8 n <= 259, u16 n <= 9318, u32 n <= 9318, u64 n <= 9318,
 *            u128 n <= 9318, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* The offset for A062173 is 1, so n must be ≥ 1. If called with n == 0
       (outside the defined range) we return 0 – the caller should not rely
       on this value. */
    if (n == 0)
        return (seq_t)0;

    /* Compute 2^(n-1) modulo n using the generic modular exponentiation
       routine provided by the build utilities. */
    return mod_pow((seq_t)2, n - 1, (seq_t)n);
}
