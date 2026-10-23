/*
 * A029817  Average theta series of odd unimodular lattices of dimension 16
 *          (multiplied by 17).
 * https://oeis.org/A029817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 17 a(n) = 32 * Σ_{d|n} d^7 * (-1)^{n-d} for n ≥ 1 (The
 *            sum runs over all divisors d of n. The sign is +1 if n and d
 *            have the same parity, and −1 otherwise.)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow7_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include "../utils/pow7_uint128.h"

seq_t a(idx_t n) {
    if (n == 0) {
        /* a(0) = 17 as given */
        return (seq_t)17;
    }

    /* Use 128‑bit temporaries to avoid overflow in intermediate sums. */
    __uint128_t pos = 0;   /* sum of positive terms  */
    __uint128_t neg = 0;   /* sum of negative terms  */

    /* Loop over divisors d of n up to sqrt(n). */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) {
            continue;
        }
        idx_t e = n / d;          /* complementary divisor */

        /* term for divisor d */
        __uint128_t val_d = pow7_uint128(d);
        /* sign for d : (+1) if n and d have same parity, else (-1) */
        if (((n & 1) ^ (d & 1)) == 0) {
            pos += val_d;
        } else {
            neg += val_d;
        }

        if (e == d) {
            /* Perfect square – already handled */
            continue;
        }

        /* term for complementary divisor e */
        __uint128_t val_e = pow7_uint128(e);
        if (((n & 1) ^ (e & 1)) == 0) {
            pos += val_e;
        } else {
            neg += val_e;
        }
    }

    /* Compute the signed sum: pos − neg  (should be non‑negative). */
    __uint128_t diff = (pos > neg) ? (pos - neg) : (neg - pos);

    /* Multiply by 32 and cast back to seq_t. */
    __uint128_t scaled = diff * ( (__uint128_t)32 );
    return (seq_t)scaled;
}
