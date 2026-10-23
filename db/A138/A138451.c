/*
 * A138451  a(n) = (prime(n)^6-prime(n)^2)/60.
 * https://oeis.org/A138451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (prime(n)^6 - prime(n)^2)/60 (OEIS A138451)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 22, u64 n <= 454, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- Prime generator (cached sieve) ---------- */
static uint64_t *prime_list = NULL;
static size_t   prime_count = 0;
static idx_t    sieve_limit = 0;   /* highest bound currently sieved */

/* recompute all primes up to the given limit, replacing the existing list */
static void recompute_primes(idx_t limit)
{
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime)
        return;                      /* leave current list as is on allocation failure */

    /* initialize */
    for (size_t i = 0; i <= (size_t)limit; ++i)
        is_prime[i] = true;
    if (limit >= 0) {
        is_prime[0] = false;
        if (limit >= 1)
            is_prime[1] = false;
    }

    /* Sieve of Eratosthenes */
    for (size_t p = 2; (size_t)p * p <= (size_t)limit; ++p) {
        if (is_prime[p]) {
            for (size_t q = p * p; q <= (size_t)limit; q += p)
                is_prime[q] = false;
        }
    }

    /* count primes */
    size_t count = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i])
            ++count;

    /* allocate new list */
    uint64_t *new_list = (uint64_t *)realloc(prime_list, count * sizeof(uint64_t));
    if (!new_list) {
        free(is_prime);
        return;                      /* leave current list on allocation failure */
    }
    prime_list = new_list;
    prime_count = count;

    /* copy primes */
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (is_prime[i])
            prime_list[idx++] = (uint64_t)i;
    }

    sieve_limit = limit;
    free(is_prime);
}

/* ensure that at least n primes are available in prime_list */
static void ensure_n_primes(idx_t n)
{
    if ((size_t)n <= prime_count)
        return;                      /* already have enough */

    /* upper bound for the nth prime – safe for n up to ~10 000 */
    idx_t limit;
    if (n < 6)
        limit = 15;
    else
        limit = n * (idx_t)20 + 10;   /* 20*n + 10 is well above the actual nth prime */

    if (limit > sieve_limit)          /* need to expand the sieve */
        recompute_primes(limit);
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* defined arbitrarily for completeness */

    ensure_n_primes(n);
    if (n > (idx_t)prime_count)       /* allocation error fallback */
        return (seq_t)0;

    uint64_t p = prime_list[(size_t)n - 1];   /* nth prime */

    /* Branch depending on the size of seq_t */
    if (sizeof(seq_t) <= 8) {
        /* Use unsigned __int128 to avoid overflow */
        unsigned __int128 x = (unsigned __int128)p;
        unsigned __int128 p2 = x * x;         /* p^2   */
        unsigned __int128 p4 = p2 * p2;       /* p^4   */
        unsigned __int128 p6 = p4 * p2;       /* p^6   */
        unsigned __int128 val = (p6 - p2) / (unsigned __int128)60;
        return (seq_t)val;                   /* fits in seq_t */
    } else {
        /* Use seq_t arithmetic (handles arbitrary‑precision) */
        seq_t sp = (seq_t)p;
        seq_t p2 = sp * sp;                  /* p^2   */
        seq_t p4 = p2 * p2;                  /* p^4   */
        seq_t p6 = p4 * p2;                  /* p^6   */
        seq_t val = (p6 - p2) / (seq_t)60;
        return val;
    }
}
