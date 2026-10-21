/*
 * A234694  a(n) = |{0 < k < n: p = k + prime(n-k) and prime(p) - p + 1 are
 *          both prime}|.
 * https://oeis.org/A234694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 < k < n : p = k + prime(n-k) and prime(p)-p+1 is
 *            prime}|
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Utility prototypes that may be provided by the build system (do not redefine) */
#ifdef __cplusplus
extern "C" {
#endif
extern bool is_prime_small(uint64_t n);
#ifdef __cplusplus
}
#endif

/* Static data for prime table and sieve */
static char *sieve_table = NULL;          /* 1 = prime, 0 = composite */
static uint32_t *prime_list = NULL;       /* prime_list[0] = 2, etc. */
static size_t  prime_count   = 0;
static size_t  sieve_limit   = 0;         /* maximum value stored in sieve */

/* One‑time initialization of the sieve and list of primes. */
static void init_prime_tables(void)
{
    if (sieve_table != NULL)
        return;                         /* already initialised */

    /* Choose a limit that comfortably covers all needed indices.
       For n <= 10 000 the largest candidate p is < 115 000,
       and prime(p) < 1.6 million.  A limit of 3 million suffices. */
    const size_t LIMIT = 3000000;
    sieve_limit = LIMIT;

    sieve_table = (char *)malloc(LIMIT + 1);
    if (!sieve_table)
        abort();
    memset(sieve_table, 1, LIMIT + 1);   /* assume prime */
    sieve_table[0] = sieve_table[1] = 0; /* 0 and 1 are not prime */

    /* Classic Eratosthenes sieve. */
    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (sieve_table[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                sieve_table[j] = 0;

    /* Count primes to allocate the list. */
    size_t cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (sieve_table[i])
            ++cnt;
    prime_count = cnt;

    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_list)
        abort();
    for (size_t i = 2, pos = 0; i <= LIMIT && pos < cnt; ++i)
        if (sieve_table[i])
            prime_list[pos++] = (uint32_t)i;
}

/* Return the k‑th prime (1‑based).  Assumes k >= 1. */
static inline uint32_t prime_k(idx_t k)
{
    return prime_list[(size_t)(k - 1)];
}

/* Fast primality test for numbers up to sieve_limit. */
static inline bool is_prime_small_uint32(uint32_t x)
{
    return x <= sieve_limit ? sieve_table[x] : false;
}

/* Compute a single value a(n). */
static seq_t compute_a_value(idx_t n)
{
    if (n <= 2) {                     /* a(1)=0, a(2)=1 by definition */
        return (seq_t)(n == 1 ? 0 : 1);
    }

    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        idx_t idx = n - k;            /* 1 <= idx <= n-1 */
        uint32_t pk  = prime_k(idx);  /* prime(n-k) */
        uint32_t p   = (uint32_t)k + pk;   /* candidate p */

        if (!is_prime_small_uint32(p))
            continue;                 /* p must be prime */

        uint32_t qp   = prime_k(p);   /* prime(p) */
        uint32_t val  = qp - p + 1;    /* prime(p)-p+1 */

        if (is_prime_small_uint32(val))
            ++count;
    }
    return count;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* behaviour below offset not required */
        return (seq_t)0;
    init_prime_tables();
    return compute_a_value(n);
}
