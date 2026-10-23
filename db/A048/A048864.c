/*
 * A048864  Number of nonprime numbers (composites and 1) in the reduced
 *          residue system of n.
 * https://oeis.org/A048864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n) - (pi(n) - A001221(n)) = phi(n) - pi(n) +
 *            A001221(n) where phi is Euler's totient function and pi counts
 *            primes ≤ n
 * Range:     u8 n <= 329, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static uint32_t *primes = NULL;   /* list of primes up to current capacity   */
static size_t    primeCount = 0;   /* number of primes stored                */
static idx_t     capacity    = 0;   /* maximum n for which primes are known   */

/* Build or extend the prime list up to n (inclusive). */
static void ensure_primes(idx_t n)
{
    if (n <= capacity)
        return;

    size_t sz = (size_t)n + 1;               /* allocate array for marking   */
    bool *isPrime = (bool *)malloc(sz);
    for (size_t i = 0; i < sz; ++i)
        isPrime[i] = true;
    isPrime[0] = isPrime[1] = false;

    for (size_t p = 2; p * p <= n; ++p) {
        if (isPrime[p]) {
            for (size_t m = p * p; m <= n; m += p)
                isPrime[m] = false;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= n; ++i)
        if (isPrime[i])
            ++cnt;

    /* allocate and fill list */
    uint32_t *newprimes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= n; ++i)
        if (isPrime[i])
            newprimes[idx++] = (uint32_t)i;

    free(isPrime);
    free(primes);
    primes = newprimes;
    primeCount = cnt;
    capacity = n;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined for n < 1 */

    ensure_primes(n);

    /* Compute phi(n) and count distinct prime factors. */
    idx_t temp   = n;
    seq_t phi    = (seq_t)temp;
    seq_t distinct = 0;

    for (size_t i = 0; i < primeCount; ++i) {
        uint32_t p = primes[i];
        if ((idx_t)p * p > temp)
            break;
        if (temp % p == 0) {
            ++distinct;
            phi = phi * ((seq_t)(p - 1)) / (seq_t)p;  /* exact division     */
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1) {              /* remaining prime factor > sqrt(n) */
        ++distinct;
        phi = phi * ((seq_t)(temp - 1)) / (seq_t)temp;
    }

    /* Count primes ≤ n. */
    idx_t pi = 0;
    for (size_t i = 0; i < primeCount; ++i)
        if (primes[i] <= n)
            ++pi;

    return phi - (seq_t)pi + distinct;
}
