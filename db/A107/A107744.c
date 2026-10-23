/*
 * A107744  Smallest prime factor of 6*n+1.
 * https://oeis.org/A107744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime factor of (6*n + 1)
 * Range:     u8 n <= 44, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 6*n+1 fits in 64‑bit for n up to 10000, so we compute it in
       a 64‑bit unsigned integer and then cast to seq_t for the result. */
    uint64_t p = 6ull * n + 1ull;

    /* Find the smallest divisor > 1 by trial division. */
    for (uint64_t i = 2; i * i <= p; ++i) {
        if (p % i == 0)
            return (seq_t)i;          /* i is a prime factor */
    }

    /* If no divisor was found, p itself is prime. */
    return (seq_t)p;
}
