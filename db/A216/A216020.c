/*
 * A216020  Primes that are the sum of 100001 consecutive primes.
 * https://oeis.org/A216020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: prime that equals sum of 100001 consecutive primes */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* constants */
#define SIEVE_LIMIT 20000000   /* 20 million, enough for n ≤ 10000 */
#define BLOCK_SIZE 100001      /* length of the consecutive block */

/* helper function prototypes */
static void init_primes(void);
static bool is_prime_u64(uint64_t n);
static uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod);
static uint64_t pow_mod64(uint64_t a, uint64_t d, uint64_t mod);

/* static storage */
static bool initialized = false;
static uint32_t *primes = NULL;          /* list of primes (uint32_t is enough) */
static size_t primes_cnt = 0;
static seq_t *prefix = NULL;             /* prefix sums: prefix[i+1] = sum of first i primes, prefix[0] = 0 */
static seq_t *found = NULL;              /* found primes that are sums of 100001 consecutive primes */
static size_t found_cnt = 0;
static size_t found_cap = 0;
static size_t last_start = 0;

/* allocate and generate primes up to SIEVE_LIMIT, compute prefix sums */
static void init_primes(void)
{
    /* estimate number of primes ≤ SIEVE_LIMIT: π(x) < x / log x for x≥ 2 */
    size_t cap = SIEVE_LIMIT / 10;          /* rough initial capacity */
    primes = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!primes) abort();

    prefix = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
    if (!prefix) abort();

    prefix[0] = 0;

    /* simple sieve using a byte array */
    uint8_t *is_comp = (uint8_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint8_t));
    if (!is_comp) abort();

    size_t cnt = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            /* store prime */
            if (cnt >= cap) {
                cap <<= 1;
                primes = (uint32_t *)realloc(primes, cap * sizeof(uint32_t));
                if (!primes) abort();
                prefix = (seq_t *)realloc(prefix, (cap + 1) * sizeof(seq_t));
                if (!prefix) abort();
            }
            primes[cnt++] = i;
            prefix[cnt] = prefix[cnt - 1] + (seq_t)i;

            /* mark multiples */
            if ((uint64_t)i * i <= SIEVE_LIMIT) {
                for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);

    primes_cnt = cnt;
    /* shrink arrays to actual size */
    primes = (uint32_t *)realloc(primes, primes_cnt * sizeof(uint32_t));
    if (!primes) abort();
    prefix = (seq_t *)realloc(prefix, (primes_cnt + 1) * sizeof(seq_t));
    if (!prefix) abort();
}

/* modular multiplication using 128‑bit intermediate */
static uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* modular exponentiation */
static uint64_t pow_mod64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t r = 1;
    a %= mod;
    while (d) {
        if (d & 1) r = mul_mod64(r, a, mod);
        a = mul_mod64(a, a, mod);
        d >>= 1;
    }
    return r;
}

/* deterministic Miller–Rabin for 64‑bit integers */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;

    /* small primes */
    const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* write n-1 as d*2^s */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* bases sufficient for 64-bit integers */
    const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* append a found value to the dynamic array */
static void append_found(seq_t val)
{
    if (found_cnt >= found_cap) {
        found_cap = found_cap ? found_cap << 1 : 8;
        seq_t *tmp = (seq_t *)realloc(found, found_cap * sizeof(seq_t));
        if (!tmp) abort();
        found = tmp;
    }
    found[found_cnt++] = val;
}

/* required public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1 */

    if (!initialized) {
        init_primes();
        initialized = true;
    }

    while (found_cnt < n) {
        if (last_start + BLOCK_SIZE > primes_cnt) {
            /* should not happen with the chosen sieve limit */
            abort();
        }
        seq_t sum = prefix[last_start + BLOCK_SIZE] - prefix[last_start];
        if (is_prime_u64((uint64_t)sum))
            append_found(sum);
        ++last_start;
    }
    return found[n - 1];
}
