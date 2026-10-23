/*
 * A017711  Numerator of sum of -24th powers of divisors of n.
 * https://oeis.org/A017711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = sigma_24(n) / gcd(sigma_24(n), n^24)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 40,
 *            big any n
 * Helpers:   gcd, seq_pow, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerator of the reduced fraction sigma_24(n)/n^24            */

#include "../utils/sigma_k.h"
#include "../utils/gcd.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for 0, but protect guard */

    /* numerator = σ₍₂₄₎(n) */
    seq_t num = sigma_k(n, 24);

    /* denominator = n²⁴ */
    seq_t denom = seq_pow((seq_t)n, 24);

    /* common divisor */
    seq_t g = gcd(num, denom);

    /* reduced numerator */
    return num / g;
}
