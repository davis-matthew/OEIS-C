/*
 * A131491  a(n) = 2*prime(n)!.
 * https://oeis.org/A131491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * prime(n)!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   factorial, get_prime, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/factorial.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    /* Get the nth prime (1‑based). */
    uint32_t p = get_prime(n);
    /* Compute p! as a seq_t. */
    seq_t f = factorial((idx_t)p);
    /* Multiply by 2. */
    return mul_small(f, 2);
}
