/*
 * A095997  Number of divisors of n! that are coprime to n.
 * https://oeis.org/A095997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏{p≤n, p∤n} ( e_p(n!) + 1 ), where e_p(n!) = Σ_{k≥1} ⌊ n
 *            / p^k ⌋
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 52, u64 n <= 126,
 *            u128 n <= 292, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>

/* --- static cache of primes up to the largest n requested --- */
static idx_t  prime_limit = 0;      /* maximal n handled so far   */
static uint64_t *prime_array = NULL;/* list of primes up to prime_limit */
static size_t   prime_count = 0;    /* number of primes in prime_array */

/* Ensure that prime_array contains all primes ≤ n.
   Uses a simple Eratosthenes sieve; re‑allocates the array when a larger
   limit is needed. */
static void
ensure_primes_up_to(idx_t n)
{
    if (prime_limit >= n) return;   /* already sufficient */

    idx_t limit = n;
    /* allocate sieve: cast needed for C++ compilation */
    uint8_t *is_prime = (uint8_t *) calloc(limit + 1, sizeof(uint8_t));
    if (!is_prime) return;            /* out of memory – should never happen */

    /* initialise */
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = 1;

    /* sieve */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* count primes to reallocate array once */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint64_t *new_primes = (uint64_t *) malloc(cnt * sizeof(uint64_t));
    if (!new_primes) {
        free(is_prime);
        return;   /* allocation failed – should never happen */
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_primes[idx++] = (uint64_t) i;

    free(is_prime);

    /* replace old array */
    free(prime_array);
    prime_array = new_primes;
    prime_count = cnt;
    prime_limit  = limit;
}

/* --- public sequence function -------------------------------------- */
seq_t
a(idx_t n)
{
    /* a(1) = 1 : empty product */
    if (n < 2) return 1;

    ensure_primes_up_to(n);

    seq_t result = 1;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_array[i];
        if (p > (uint64_t)n) break;
        if (n % p == 0) continue;    /* skip primes dividing n */

        /* exponent of p in n! via Legendre's formula */
        idx_t t = n;
        idx_t e = 0;
        while (t) {
            t /= p;
            e += t;
        }
        result *= (seq_t)(e + 1);
    }

    return result;
}
