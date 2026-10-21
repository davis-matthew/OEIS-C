/*
 * A070230  Let S_n ={p_n + q : p_n is the n-th odd prime and q runs through
 *          odd primes not greater than p_n}; sequence gives size of S_n
 *          intersect S_(n+1).
 * https://oeis.org/A070230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |{ primes q ≤ p_n : q – (p_{n+1}–p_n) is prime }|
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here p_k denotes the k‑th odd prime (3,5,7,11,…).           */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Sieve of Eratosthenes up to a fixed limit.  That limit (2 000 000) is
 *  large enough for n up to about 10 000 (the 10 000‑th odd prime is <115 500).
 *  The sieve is built once on the first call to a().
 */
#define SIEVE_LIMIT 2000000U

/* global data (initialised on first use) */
static unsigned char *is_prime = NULL;      /* 1 if i is prime, 0 otherwise */
static uint32_t   *odd_primes = NULL;       /* list of odd primes, 3,5,7,…      */
static size_t     odd_primes_cnt = 0;        /* number of odd primes stored     */

/* initialise the sieve and build the odd‑prime list */
static void init_sieve(void) {
    if (is_prime != NULL)
        return;                           /* already initialised */

    /* --------- 0/1 array for primality -------------------------------- */
    is_prime = (unsigned char*)calloc(SIEVE_LIMIT + 1, sizeof(unsigned char));
    if (is_prime == NULL)
        exit(EXIT_FAILURE);
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        is_prime[i] = 1;
    for (size_t p = 2; p * p <= SIEVE_LIMIT; ++p)
        if (is_prime[p]) {
            for (size_t j = p * p; j <= SIEVE_LIMIT; j += p)
                is_prime[j] = 0;
        }

    /* --------- count odd primes ---------------------------------------- */
    odd_primes_cnt = 0;
    for (uint32_t i = 3; i <= SIEVE_LIMIT; i += 2)
        if (is_prime[i])
            odd_primes_cnt++;

    odd_primes = (uint32_t*)malloc(odd_primes_cnt * sizeof(uint32_t));
    if (odd_primes == NULL)
        exit(EXIT_FAILURE);
    size_t pos = 0;
    for (uint32_t i = 3; i <= SIEVE_LIMIT; i += 2)
        if (is_prime[i])
            odd_primes[pos++] = i;
}

/* --------------------------------------------------------------------- */
/*  compute a(n) for the definition above.  The function is thread‑safe
 *  only through a single initialization; subsequent calls are fast.
 */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;                      /* undefined for offset < 1 */

    init_sieve();                     /* ensure data are ready */

    /* bounds check – if one asks for more primes than we have, return 0 */
    if (n + 1 > odd_primes_cnt)
        return 0;

    uint32_t p_n    = odd_primes[(size_t)(n - 1)];   /* p_n   */
    uint32_t p_next = odd_primes[(size_t)n];        /* p_{n+1} */
    uint32_t gap    = p_next - p_n;                /* even, >= 2 */

    /* we need only primes q >= gap + 3, otherwise q-gap < 3 */
    uint32_t min_q = gap + 3;

    seq_t count = 0;
    for (size_t idx = 0; idx < odd_primes_cnt && odd_primes[idx] <= p_n; ++idx) {
        uint32_t q = odd_primes[idx];
        if (q < min_q)          /* r = q-gap would be < 3 or underflow */
            continue;

        uint32_t r = q - gap;   /* r is odd and >= 3 */
        if (r <= SIEVE_LIMIT && is_prime[r])
            count++;
    }
    return count;
}
