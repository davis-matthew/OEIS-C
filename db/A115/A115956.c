/*
 * A115956  Numbers k having exactly 2 distinct prime factors, the largest of
 *          which is greater than or equal to sqrt(k) (i.e., sqrt(k)-rough
 *          numbers with exactly 2 distinct prime factors).
 * https://oeis.org/A115956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers with exactly two distinct prime factors and the largest
 *            prime factor >= sqrt(k)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static bool primes_ready = false;
static bool terms_ready  = false;
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;

static seq_t *terms = NULL;
static size_t terms_cnt = 0;
static size_t terms_cap = 0;

static uint64_t current_k = 6;      /* next integer to test */

/* integer square root: floor(sqrt(n)) */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t res  = 0;
    uint64_t bit  = 1ULL << 62;          /* 2^62 is the largest power of four <= 2^64 */

    while (bit > n) bit >>= 2;

    while (bit != 0) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* prepare a list of primes up to LIMIT (inclusive). */
static void init_primes(void)
{
    const uint32_t LIMIT = 2000000;          /* limit large enough for our needs */
    bool *isPrime = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    if (!isPrime) return;

    for (uint32_t i = 0; i <= LIMIT; ++i) isPrime[i] = true;
    isPrime[0] = isPrime[1] = false;

    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (isPrime[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                isPrime[j] = false;
        }
    }

    primes_cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (isPrime[i]) primes_cnt++;

    primes = (uint32_t*)malloc(primes_cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (isPrime[i]) primes[idx++] = i;

    free(isPrime);
    primes_ready = true;
}

/* enlarge terms array when needed */
static void add_term(uint64_t v)
{
    if (terms_cnt == terms_cap) {
        terms_cap = (terms_cap == 0) ? 16 : terms_cap << 1;
        terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_cnt++] = (seq_t)v;
}

/* extend the pre‑computed sequence until it contains at least n elements */
static void extend_to(idx_t n)
{
    if (!terms_ready) {
        terms = (seq_t*)malloc(16 * sizeof(seq_t));
        terms_cap = 16;
        terms_cnt = 0;
        terms_ready = true;
    }

    while (terms_cnt < (size_t)n) {
        uint64_t m = current_k;
        uint64_t rem = m;
        int distinct = 0;
        uint64_t largest = 0;

        /* factorization */
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * (uint64_t)p > rem) break;
            if (rem % p == 0) {
                ++distinct;
                if (p > largest) largest = p;
                while (rem % p == 0) rem /= p;
                if (distinct > 2) break;
            }
        }

        if (distinct <= 2 && rem > 1) {
            ++distinct;
            if (rem > largest) largest = rem;
            if (distinct > 2) {
                current_k++;
                continue;
            }
        }

        if (distinct == 2) {
            uint64_t floor_sqrt = isqrt_uint64(current_k);
            uint64_t ceil_sqrt = floor_sqrt;
            if (floor_sqrt * floor_sqrt < current_k) ceil_sqrt++;

            if (largest >= ceil_sqrt)
                add_term(current_k);
        }

        current_k++;
    }
}

seq_t a(idx_t n)
{
    if (!primes_ready) init_primes();
    extend_to(n);
    return terms[n - 1];
}
