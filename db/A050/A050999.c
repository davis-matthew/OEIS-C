/*
 * A050999  Sum of squares of odd divisors of n.
 * https://oeis.org/A050999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sum of squares of odd divisors of n. For n odd: a(n) =
 *            σ₂(n). For n even: a(n) = σ₂(n) – 4·σ₂(n/2)
 * Range:     u8 n <= 14, u16 n <= 242, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"   /* provides seq_t sigma_k(idx_t m, uint8_t k); */

seq_t a(idx_t n)
{
    /* n >= 1. */
    if ((n & 1) == 1) {
        /* Odd n: all divisors are odd. */
        return sigma_k(n, 2);
    } else {
        /* Even n: use a(2m) = σ₂(2m) – 4·σ₂(m). */
        seq_t sigma_n   = sigma_k(n, 2);
        seq_t sigma_half = sigma_k(n / 2, 2);
        return sigma_n - (seq_t)4 * sigma_half;
    }
}
