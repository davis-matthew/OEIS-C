/*
 * A101206  Third column of triangle in A101204.
 * https://oeis.org/A101206
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! - 2^(prime(n-3)-2) for n ≥ 4, a(2) = 1, a(3) = 2
 *            (the first terms)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   get_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/seq_pow.h"

/* Computes a(n) for the OEIS sequence A101206. */
seq_t a(idx_t n)
{
    /* Handle the small offsets explicitly. */
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;

    /* Compute (n-1)! in seq_t. */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n - 1; ++i)
        fact *= (seq_t)i;

    /* Compute exponent: prime(n-3) - 2.  get_prime(idx) returns seq_t. */
    seq_t prime_val = get_prime(n - 3);
    /* Convert to idx_t for seq_pow; the prime fits in 64‑bit. */
    idx_t exp = (idx_t)(prime_val) - 2;

    /* Compute 2^exp. */
    seq_t pow2 = seq_pow((seq_t)2, exp);

    /* a(n) = fact - pow2. */
    return fact - pow2;
}
