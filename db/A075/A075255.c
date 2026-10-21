/*
 * A075255  a(n) = n - (sum of prime factors of n (with repetition)).
 * https://oeis.org/A075255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A001414(n) where A001414(n) = sum of prime factors of
 *            n (with repetition)
 * Range:     u8 n <= 279, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for all n >= 1. */
seq_t a(idx_t n)
{
    /* copy of n for factorisation */
    idx_t rem = n;
    seq_t sum = 0;               /* sum of prime factors with multiplicity */

    /* trial division up to sqrt(rem) */
    for (idx_t p = 2; p <= rem / p; ++p) {
        while (rem % p == 0) {
            sum += (seq_t)p;
            rem /= p;
        }
    }
    /* if a prime factor remains */
    if (rem > 1) {
        sum += (seq_t)rem;
    }

    /* a(n) = n - sum_of_prime_factors */
    return (seq_t)n - sum;
}
