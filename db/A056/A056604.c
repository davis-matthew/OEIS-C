/*
 * A056604  a(0)=1; thereafter a(n) = lcm(1, 2, 3, 4, ..., prime(n)).
 * https://oeis.org/A056604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A056604:  lcm(1,2,…,prime(n))
 *  Formula: a(n) = Π_{k=1..n} prime(k)^⌊log(prime(n)) / log(prime(k))⌋
 *  a(0) = 1
 *  Here we sieve primes on demand and multiply the required prime powers.
 *  The function works for arbitrary seq_t (including big integers),
 *  using only integer arithmetic and seq_t multiplication.
 */

#include <stdlib.h>   /* calloc, malloc, free */

/* Global state for prime generation */
static uint32_t *prime_list = NULL;   /* primes[0] == 2 */
static size_t prime_count = 0;        /* number of primes stored */
static size_t sieve_limit   = 0;       /* highest limit used in the sieve */

/* Sieve of Eratosthenes up to 'limit', storing all primes ≤ limit. */
static void sieve_up_to(size_t limit)
{
    uint8_t *is_comp = (uint8_t *)calloc(limit + 1, 1);   /* 0 = prime candidate, 1 = composite */
    if (!is_comp) exit(1);

    for (size_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
        }
    }

    /* Count primes to allocate storage. */
    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++count;

    uint32_t *new_list = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!new_list) { free(is_comp); exit(1); }

    /* Fill the list. */
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) new_list[idx++] = (uint32_t)i;

    free(is_comp);

    /* Replace the old list. */
    free(prime_list);
    prime_list = new_list;
    prime_count = count;
    sieve_limit = limit;
}

/* Ensure that at least 'n_needed' primes are stored in prime_list. */
static void ensure_primes(size_t n_needed)
{
    if (prime_count >= n_needed) return;

    /* Start from the previous limit or 2. */
    size_t limit = (sieve_limit > 1) ? sieve_limit : 2;

    /* Rough upper bound for the n-th prime (works well for n up to 1e6). */
    size_t est = n_needed * 20 + 10;
    if (limit < est) limit = est;

    while (1) {
        sieve_up_to(limit);
        if (prime_count >= n_needed) return;
        limit *= 2;                     /* double the range until we are good */
    }
}

/* -------------------------------------------------------------------------- */
/*  The sequence function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;               /* a(0) = 1 */

    /* Fetch first n primes (1‑based index). */
    ensure_primes((size_t)n);
    uint32_t p_n = prime_list[(size_t)n - 1];   /* n‑th prime */

    seq_t result = (seq_t)1;

    /* Compute the product of prime powers. */
    for (size_t i = 0; i < (size_t)n; ++i) {
        uint32_t p = prime_list[i];

        /* Find largest e such that p^e ≤ p_n. */
        unsigned long long power = p;
        int e = 1;
        while (power <= p_n / p) {
            power *= p;
            ++e;
        }

        /* Compute p^e as a seq_t and multiply into result. */
        seq_t term = (seq_t)p;
        for (int j = 1; j < e; ++j)
            term *= (seq_t)p;

        result *= term;
    }

    return result;
}
