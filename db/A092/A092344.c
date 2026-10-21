/*
 * A092344  a(0)=1; a(n) = sigma_2(n) + sigma_3(n).
 * https://oeis.org/A092344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = σ₂(n) + σ₃(n)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1535, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* σ_k(n) is the sum of the k‑th powers of the positive divisors of n. */
#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    seq_t s2 = sigma_k(n, 2U);
    seq_t s3 = sigma_k(n, 3U);
    return s2 + s3;
}
