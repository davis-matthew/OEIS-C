/*
 * A075134  Indices of triple-safe primes: p=prime(n) is double-safe:
 *          q=(p-1)/2, r=(q-1)/2 and s=(r-1)/2 are all prime (and q is
 *          double-safe).
 * https://oeis.org/A075134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by sieving primes up to a fixed limit and selecting
   indices of primes p where p, (p-1)/2, (p-1)/2/2 and (p-1)/2/2/2
   are all prime (triple‑safe). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Upper limit for the sieve. 300 million bits ≈ 37.5 MiB.  This
   covers all tests up to n≈10 000. */
#define SIEVE_LIMIT (uint32_t)300000000U

/* Bit sieve: 0 = prime, 1 = composite. */
static uint8_t *sieve = NULL;
static size_t sieve_bytes = 0;

/* Prime list. */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap = 0;

/* Triple‑safe prime indices. */
static uint32_t *indices = NULL;
static size_t indices_len = 0;
static size_t indices_cap = 0;

/* Flag whether initialization has been done. */
static bool inited = false;

static inline void set_composite(uint32_t n)
{
    sieve[n >> 3] |= (uint8_t)(1U << (n & 7U));
}

static inline bool is_composite(uint32_t n)
{
    return (sieve[n >> 3] >> (n & 7U)) & 1U;
}

static void init_sequence(void)
{
    if (inited) return;

    /* Allocate sieve bits. */
    sieve_bytes = (SIEVE_LIMIT >> 3) + 1;
    sieve = (uint8_t *)malloc(sieve_bytes);
    if (!sieve) return;               /* allocation failure; silently ignore */
    for (size_t i = 0; i < sieve_bytes; ++i) sieve[i] = 0;

    /* 0 and 1 are not prime. */
    set_composite(0);
    set_composite(1);

    /* Mark even composites >2. */
    for (uint32_t i = 4; i <= SIEVE_LIMIT; i += 2)
        set_composite(i);

    /* Sieve odds. */
    for (uint32_t i = 3; (uint64_t)i * i <= SIEVE_LIMIT; i += 2) {
        if (is_composite(i)) continue;
        uint32_t step = i << 1;               /* skip even multiples */
        for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += step)
            set_composite(j);
    }

    /* Build prime list. */
    prime_cap = 2000000;
    prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    if (!prime_list) {
        inited = true;  /* bail out safely */
        return;
    }
    for (uint32_t n = 2; n <= SIEVE_LIMIT; ++n) {
        if (!is_composite(n)) {
            if (prime_count == prime_cap) {
                prime_cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
                if (!tmp) break;
                prime_list = tmp;
            }
            prime_list[prime_count++] = n;
        }
    }

    /* Find triple‑safe prime indices. */
    indices_cap = 100000;
    indices = (uint32_t *)malloc(indices_cap * sizeof(uint32_t));
    if (!indices) {
        inited = true;
        return;
    }

    for (size_t idx = 0; idx < prime_count; ++idx) {
        uint32_t p = prime_list[idx];
        uint32_t q = (p - 1) >> 1;          /* (p-1)/2 */
        if (q < 2 || is_composite(q)) continue;
        uint32_t r = (q - 1) >> 1;          /* (q-1)/2 */
        if (r < 2 || is_composite(r)) continue;
        uint32_t s = (r - 1) >> 1;          /* (r-1)/2 */
        if (s < 2 || is_composite(s)) continue;

        if (indices_len == indices_cap) {
            indices_cap <<= 1;
            uint32_t *tmp = (uint32_t *)realloc(indices, indices_cap * sizeof(uint32_t));
            if (!tmp) break;
            indices = tmp;
        }
        /* prime index is idx+1 because prime_list is 0‑based. */
        indices[indices_len++] = (uint32_t)(idx + 1);
    }

    inited = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for n = 0, safe fallback */

    init_sequence();

    if (n > (idx_t)indices_len) return (seq_t)0;   /* out of bounds */

    return (seq_t)indices[n - 1];
}
