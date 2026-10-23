/*
 * A048753  Composite numbers k whose product of aliquot divisors divided by
 *          number of aliquot divisors is an integer.
 * https://oeis.org/A048753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 7734, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A048753
 *  Composite numbers k whose product of aliquot (proper) divisors divided
 *  by the number of aliquot divisors is an integer.
 *
 *  The function a(idx_t n) returns the n‑th term (1‑based indexing).
 *  It is memoised and works for any n≥1.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- helpers ---------- */

static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    if (mod <= 1u) return 0u;
    uint64_t result = 1u % mod;
    base %= mod;
    while (exp) {
        if (exp & 1u) result = ( (__uint128_t)result * base ) % mod;
        base = ( (__uint128_t)base * base ) % mod;
        exp >>= 1u;
    }
    return result;
}

static uint64_t isqrt_u64(uint64_t n) {
    uint64_t low = 0u, high = n;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * (__uint128_t)mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1u;
        else high = mid - 1u;
    }
    return high;
}

/* ---------- prime list (up to 200 000) ---------- */

#define PRIME_MAX 200000u

static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static bool primes_initialized = false;

static void init_primes(void) {
    bool *is_composite = (bool*)calloc(PRIME_MAX + 1, sizeof(bool));
    if (!is_composite) exit(1);

    for (size_t i = 2u; (uint64_t)i * (uint64_t)i <= PRIME_MAX; ++i) {
        if (!is_composite[i]) {
            for (size_t j = i * i; j <= PRIME_MAX; j += i)
                is_composite[j] = true;
        }
    }

    for (size_t i = 2u; i <= PRIME_MAX; ++i)
        if (!is_composite[i]) ++primes_cnt;

    primes = (uint32_t*)malloc(primes_cnt * sizeof(uint32_t));
    if (!primes) exit(1);

    size_t idx = 0u;
    for (size_t i = 2u; i <= PRIME_MAX; ++i)
        if (!is_composite[i]) primes[idx++] = (uint32_t)i;

    free(is_composite);
}

/* ---------- divisor count ---------- */

static uint64_t divisor_count_uint64(uint64_t n) {
    if (n <= 1u) return 1u;
    uint64_t total = 1u;
    uint64_t tmp = n;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > tmp) break;
        uint64_t exp = 0u;
        while (tmp % p == 0u) {
            tmp /= p;
            ++exp;
        }
        if (exp) total *= (exp + 1u);
    }
    if (tmp > 1u) total *= 2u;
    return total;
}

/* ---------- cache for previous terms ---------- */

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;

/* ensure cache can hold required number of terms */
static void grow_cache(size_t required) {
    if (required <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < required) new_cap <<= 1;
    seq_t *new_buf = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf) exit(1);
    cache = new_buf;
    cache_cap = new_cap;
}

/* ---------- main function ---------- */

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;        /* offset is 1 */

    if (!primes_initialized) { init_primes(); primes_initialized = true; }

    static uint64_t candidate = 4u; /* next number to test */

    while (cache_size < (size_t)n) {
        if (candidate < 4u) { candidate = 4u; }   /* safety */

        /* compute divisor count */
        uint64_t d = divisor_count_uint64(candidate);
        if (d > 2u) {                             /* composite */
            uint64_t m = d - 1u;                  /* number of proper divisors */
            uint64_t e = d - 2u;                  /* exponent part */
            uint64_t prod_mod;

            if ((e & 1u) == 0u) {                /* e even */
                uint64_t exp = e >> 1u;
                prod_mod = mod_pow_uint64(candidate, exp, m);
            } else {                              /* e odd, root needed */
                uint64_t exp = (e - 1u) >> 1u;
                uint64_t sqrt_k = isqrt_u64(candidate);
                prod_mod = mod_pow_uint64(candidate, exp, m);
                prod_mod = ( (__uint128_t)prod_mod * (sqrt_k % m) ) % m;
            }

            if (prod_mod == 0u) {                 /* satisfies condition */
                grow_cache(cache_size + 1);
                cache[cache_size++] = (seq_t)candidate;
            }
        }

        ++candidate;
    }

    return cache[n - 1];
}
