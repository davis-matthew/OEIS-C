/*
 * A119707  Number of distinct primes appearing in all partitions of n into
 *          prime parts.
 * https://oeis.org/A119707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=1, a(3)=1; for n≥4, a(n)=π(n) if n odd, a(n)=π(n-2)
 *            if n even
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code works both as C11 and as C++17.  The types seq_t and idx_t
   are supplied by the build system; only standard integer types may be
   used for indices or loop counters. */
#include <stdint.h>
#include <stdlib.h>

/* Sieve of Eratosthenes for primes up to the current maximum.  The
   sieve is rebuilt only when a larger 'n' is requested.  The array
   holds 1 for prime indices and 0 otherwise. */
static uint8_t *is_prime = NULL;   /* 1 if index is prime, 0 otherwise */
static idx_t   sieve_max  = 0;     /* largest n for which the sieve is valid */

/* Build or refresh the sieve up to n.  */
static void refresh_sieve(idx_t n)
{
    if (n <= sieve_max) return;

    /* Allocate a fresh sieve array; we don't try to reuse the previous
       memory because the problem size is modest (n ≈ 10⁴).  */
    uint8_t *new_sieve = (uint8_t*)calloc((size_t)n + 1, sizeof(uint8_t));
    if (!new_sieve) abort();

    for (idx_t i = 2; i <= n; ++i) new_sieve[i] = 1;

    for (idx_t i = 2; i * i <= n; ++i) {
        if (new_sieve[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                new_sieve[j] = 0;
        }
    }
    free(is_prime);
    is_prime   = new_sieve;
    sieve_max  = n;
}

/* Count primes ≤ n using the current sieve.  */
static seq_t prime_pi(idx_t n)
{
    refresh_sieve(n);
    seq_t cnt = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i]) ++cnt;
    return cnt;
}

/* OEIS sequence A119707.  */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2 || n == 3) return (seq_t)1;

    if ((n & 1) == 0)      /* even, n ≥ 4 */
        return prime_pi(n - 2);
    else                   /* odd, n ≥ 5 */
        return prime_pi(n);
}
