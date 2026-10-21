/*
 * A230850  A054541 and A000012 interleaved.
 * https://oeis.org/A230850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2, if n = 1; * a(n) = 1, if n is even (n ≥ 2); * a(n) =
 *            p_k - p_{k-1}, if n = 2k-1 ≥ 3, * where p_i is the i-th prime,
 *            p_0 = 0
 * Range:     u8 n <= 1414, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* static storage for primes */
static seq_t *primes = NULL;
static size_t primes_count = 0;
static size_t primes_cap = 0;

/* Ensure that primes vector contains at least k primes (k ≥ 1). */
static void ensure_prime_count(idx_t k)
{
    if (primes_count >= k)
        return;

    /* initial allocation and first prime */
    if (primes_count == 0) {
        primes_cap = 16;
        primes = (seq_t*)malloc(primes_cap * sizeof(seq_t));
        primes[0] = 2;
        primes_count = 1;
    }

    seq_t candidate = primes[primes_count - 1] + 1;

    while (primes_count < k) {
        bool isprime = true;
        /* Test divisibility by all known primes up to sqrt(candidate) */
        for (size_t i = 0; i < primes_count; ++i) {
            seq_t p = primes[i];
            if (p > candidate / p)
                break;                 /* p*p would overflow or exceed candidate */
            if (candidate % p == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            if (primes_count == primes_cap) {
                primes_cap *= 2;
                primes = (seq_t*)realloc(primes, primes_cap * sizeof(seq_t));
            }
            primes[primes_count++] = candidate;
        }
        ++candidate;
    }
}

/* Sequence implementation */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;

    if ((n & 1) == 0)   /* even index */
        return (seq_t)1;

    /* odd index ≥ 3 */
    idx_t k = (n + 1) / 2;        /* k ≥ 2 */
    ensure_prime_count(k);

    seq_t pk   = primes[k - 1];              /* p_k */
    seq_t pk_1 = (k == 1) ? (seq_t)0 : primes[k - 2]; /* p_{k-1} */

    return pk - pk_1;
}
