/*
 * A072613  Number of numbers of the form p*q (p, q distinct primes) less than
 *          or equal to n.
 * https://oeis.org/A072613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{p<sqrt(n)} ( Pi(n/p) - Pi(p) ) (square‑free
 *            semiprimes ≤ n)
 * Range:     u8 n <= 888, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static state for sieve, π(n) array and prime list */
static idx_t    _max_n      = 0;          /* highest value sieved so far     */
static uint8_t * _is_prime  = NULL;       /* 1 if i is prime, 0 otherwise    */
static uint32_t * _pi       = NULL;       /* π(i) for 0 ≤ i ≤ _max_n          */
static uint64_t * _primes   = NULL;       /* list of primes up to _max_n      */
static size_t    _prime_cnt = 0;          /* number of primes found           */

/* enlarge the sieve to cover at least up to 'limit' */
static void _init_sieve(idx_t limit)
{
    if (limit <= _max_n)
        return;                     /* already sufficient */

    /* free old tables if any */
    free(_is_prime);
    free(_pi);
    free(_primes);

    /* allocate new tables */
    _is_prime = (uint8_t *)malloc((limit + 1U) * sizeof(uint8_t));
    _pi       = (uint32_t *)malloc((limit + 1U) * sizeof(uint32_t));
    _primes   = (uint64_t *)malloc((limit + 1U) * sizeof(uint64_t));

    /* initialise all as prime; then correct 0 and 1 */
    for (idx_t i = 0; i <= limit; ++i)
        _is_prime[i] = 1U;
    if (limit >= 0) _is_prime[0] = 0U;
    if (limit >= 1) _is_prime[1] = 0U;

    /* Eratosthenes sieve */
    for (idx_t i = 2; i * i <= limit; ++i)
        if (_is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                _is_prime[j] = 0U;

    /* build π array and prime list */
    _prime_cnt = 0;
    _pi[0] = 0U;
    for (idx_t i = 1; i <= limit; ++i) {
        _pi[i] = _pi[i - 1] + (_is_prime[i] ? 1U : 0U);
        if (_is_prime[i])
            _primes[_prime_cnt++] = i;
    }
    _max_n = limit;
}

/* sequence function requested by the problem statement */
seq_t a(idx_t n)
{
    if (n < 2U)          /* a(1) = 0, a(0) and negative indices are out of range */
        return (seq_t)0;

    _init_sieve(n);      /* make sure the sieve covers 'n' */

    seq_t count = (seq_t)0;

    /* iterate over primes p up to sqrt(n) */
    for (size_t i = 0; i < _prime_cnt; ++i) {
        uint64_t p = _primes[i];
        if (p * p > n)   /* p > sqrt(n), stop */
            break;

        idx_t q_max = n / p;                /* largest q such that p*q <= n */
        count += (seq_t)(_pi[q_max] - _pi[p]);   /* primes q with p < q <= q_max */
    }
    return count;
}
