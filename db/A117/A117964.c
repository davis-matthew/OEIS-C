/*
 * A117964  a(n) = A117963(n) mod 2.
 * https://oeis.org/A117964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A117963(n) mod 2 A117963(n) = sum_{k=0..⌊n/2⌋} ( (C(n-k,
 *            k) mod 3 ≠ 0) ) (Legendre symbol reduces to 1 if non‑divisible
 *            by 3, 0 otherwise) Since (±1) ≡ 1 (mod 2), a(n) is the parity of
 *            the number of k with C(n-k, k) not divisible by 3
 * Range:     u8 n <= 8361, u16 n <= 8361, u32 n <= 8361, u64 n <= 8361,
 *            u128 n <= 8361, big any n
 * Helpers:   binom_mod3_nonzero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom_mod3_nonzero.h"

seq_t a(idx_t n)
{
    /* Count the parity of k with C(n-k, k) not divisible by 3. */
    seq_t parity = 0;   /* 0 or 1 */
    idx_t max_k = n >> 1;   /* floor(n/2) */

    for (idx_t k = 0; k <= max_k; ++k) {
        if (binom_mod3_nonzero(n - k, k))
            parity ^= 1;   /* toggle parity for each non‑zero term */
    }

    return parity;   /* 0 or 1 */
}
