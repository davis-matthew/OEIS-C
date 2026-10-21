/*
 * A092773  (Prime(prime(n))^2+1)/2.
 * https://oeis.org/A092773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Prime(Prime(n))^2+1)/2
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

/* Compute the n-th term of sequence A092773.
 *
 * We use the existing helper `get_prime(idx_t k)` which returns the
 * k‑th prime (1‑based indexing).  The result type `seq_t` handles
 * large integers, so the arithmetic below is safe for all indices
 * that fit in `idx_t`.  No memoisation is required – the helper
 * already caches primes internally.
 */

seq_t a(idx_t n)
{
    /* Ignore n == 0; behaviour for n < 1 is undefined per OEIS offset. */
    if (n == 0)
        return (seq_t)0;

    /* First prime: p = prime(n). */
    seq_t p = get_prime(n);

    /* Second prime index is the value of p.  The value fits in idx_t
     * for all practical n (e.g., n <= 10^7 gives p <= 200 million). */
    idx_t idx_second = (idx_t)p;

    /* Second prime: q = prime(p). */
    seq_t q = get_prime(idx_second);

    /* Compute (q^2 + 1)/2. */
    seq_t q2 = q * q;
    seq_t result = (q2 + (seq_t)1) / (seq_t)2;

    return result;
}
