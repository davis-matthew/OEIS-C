/*
 * A088018  Number of twin-prime pairs between n and 2n (inclusive).
 * https://oeis.org/A088018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{p prime | n ≤ p and p+2 ≤ 2n}, n ≥ 1 (twin‑prime pairs
 *            with both members in [n, 2n] inclusive)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ----------   global data (cached)   ---------- */
static uint8_t  *sieve_prime  = NULL;   /* sieve[i] == 1 → prime */
static uint32_t *prime_list   = NULL;   /* list of primes up to current limit */
static size_t    prime_list_sz = 0;      /* number of primes stored        */
static size_t    prime_list_cap = 0;
static uint32_t *twin_prefix  = NULL;   /* twin_prefix[i] = number of lower
                                           twin primes ≤ i                */
static size_t    twin_prefix_sz = 0;      /* size = current_limit+1          */

/* ----------   utility to grow the tables   ---------- */
static void ensure_limit(size_t limit)
{
    if (twin_prefix_sz > limit) return;     /* already large enough */

    /* --- build sieve up to 'limit' --- */
    uint8_t *new_sieve = (uint8_t*)realloc(sieve_prime, (limit + 1) * sizeof(uint8_t));
    if (!new_sieve) return;                  /* out of memory (unlikely in tests) */
    sieve_prime = new_sieve;
    memset(sieve_prime, 1, limit + 1);
    sieve_prime[0] = sieve_prime[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i)
        if (sieve_prime[i])
            for (size_t j = i * i; j <= limit; j += i)
                sieve_prime[j] = 0;

    /* --- build prime list --- */
    free(prime_list);
    prime_list_cap = limit / 10 + 10;
    prime_list_sz = 0;
    prime_list = (uint32_t*)malloc(prime_list_cap * sizeof(uint32_t));
    if (!prime_list) return;
    for (size_t i = 2; i <= limit; ++i) {
        if (sieve_prime[i]) {
            if (prime_list_sz == prime_list_cap) {
                prime_list_cap <<= 1;
                uint32_t *tmp = (uint32_t*)realloc(prime_list,
                                prime_list_cap * sizeof(uint32_t));
                if (!tmp) return;            /* memory error */
                prime_list = tmp;
            }
            prime_list[prime_list_sz++] = (uint32_t)i;
        }
    }

    /* --- build twin‑prefix array --- */
    free(twin_prefix);
    twin_prefix_sz = limit + 1;
    twin_prefix = (uint32_t*)malloc(twin_prefix_sz * sizeof(uint32_t));
    if (!twin_prefix) return;
    twin_prefix[0] = 0;
    for (size_t i = 1; i <= limit; ++i) {
        uint32_t inc = 0;
        if (i + 2 <= limit && sieve_prime[i] && sieve_prime[i + 2])
            inc = 1;
        twin_prefix[i] = twin_prefix[i - 1] + inc;
    }
}

/* ----------   the required function   ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* 0 is outside the OEIS offset */

    /* 2*n may overflow uint64_t; use 128‑bit to compute safely */
    unsigned __int128 tmp = (unsigned __int128)n * 2;
    size_t upper;
    if (tmp > SIZE_MAX) upper = SIZE_MAX;
    else                upper = (size_t)tmp;

    ensure_limit(upper);

    uint32_t cnt;
    if (n > 1)
        cnt = twin_prefix[upper] - twin_prefix[n - 1];
    else            /* n == 1 */
        cnt = twin_prefix[upper];

    return (seq_t)cnt;
}
