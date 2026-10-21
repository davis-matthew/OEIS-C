/*
 * A084375  Indices of primes which remain prime if any one digit is deleted
 *          (leading zeros allowed).
 * https://oeis.org/A084375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = primepi( p_n ) , where p_n is the n-th prime that remains
 *            prime when any one decimal digit is deleted (leading zeros after
 *            deletion are allowed). The function scans primes in increasing
 *            order, tests the deletion property, counts the good primes and
 *            returns the index of the n‑th such prime....
 * Range:     u8 n <= 19, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   ceil_sqrt_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/ceil_sqrt_uint64.h"

/* ---------- Configuration ---------------------------------------------- */
#define SEG_SIZE  1048576        /* 2^20  numbers per sieving segment  */

/* ---------- Small prime table ----------------------------------------- */
#define BASE_MAX  1000000        /* initial limit for base primes       */
static uint32_t base_primes[BASE_MAX];
static size_t   base_count = 0;

static void init_base_primes(void)
{
    if (base_count > 0) return;           /* already built */

    bool *sieve = (bool*)malloc((BASE_MAX + 1) * sizeof(bool));
    if (!sieve) return;                   /* unlikely */

    memset(sieve, 0, (BASE_MAX + 1) * sizeof(bool));
    for (uint32_t i = 2; (uint64_t)i * i <= BASE_MAX; ++i) {
        if (!sieve[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= BASE_MAX; j += i)
                sieve[(size_t)j] = true;
        }
    }
    for (uint32_t i = 2; i <= BASE_MAX; ++i)
        if (!sieve[i]) base_primes[base_count++] = i;

    free(sieve);
}

/* ---------- Extend base primes if needed ------------------------------ */
static void extend_base_primes(uint64_t needed_limit)
{
    if (needed_limit <= BASE_MAX) return;

    /* Allocate a temporary sieve up to needed_limit */
    uint32_t new_limit = (uint32_t)needed_limit;
    if (new_limit > 100000000) new_limit = 100000000;   /* safety */
    bool *sieve = (bool*)malloc((new_limit + 1) * sizeof(bool));
    if (!sieve) return;                                    /* unlikely */

    memset(sieve, 0, (new_limit + 1) * sizeof(bool));
    for (size_t i = 0; i < base_count; ++i)
        sieve[(size_t)base_primes[i]] = true;                 /* mark as prime */

    for (uint32_t i = 2; (uint64_t)i * i <= new_limit; ++i) {
        if (!sieve[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= new_limit; j += i)
                sieve[(size_t)j] = true;
        }
    }

    for (uint32_t i = 2; i <= new_limit; ++i)
        if (!sieve[i] && (base_count == 0 || i > base_primes[base_count-1]))
            if (base_count < BASE_MAX) base_primes[base_count++] = i;

    free(sieve);
}

/* ---------- 64‑bit modular arithmetic --------------------------------- */
static inline uint64_t mul64_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t pow64_mod(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = mul64_mod(result, base, mod);
        base = mul64_mod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* ---------- Deterministic Miller–Rabin for 64‑bit ---------------------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;

    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* write n-1 as d*2^s */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1ULL) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow64_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul64_mod(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- Delete‑digit prime test ------------------------------------ */
static bool has_prime_with_deletion(uint64_t p)
{
    if (p < 10) return false;         /* single digit -> deletion gives 0 */

    /* extract decimal digits into an array */
    int dig[20];
    int len = 0;
    uint64_t tmp = p;
    while (tmp > 0) {
        dig[len++] = tmp % 10;
        tmp /= 10;
    }
    /* reverse to most significant first */
    for (int i = 0; i < len/2; ++i) {
        int t = dig[i];
        dig[i] = dig[len-1-i];
        dig[len-1-i] = t;
    }

    /* test each deletion */
    for (int del = 0; del < len; ++del) {
        uint64_t val = 0;
        for (int j = 0; j < len; ++j) {
            if (j == del) continue;
            val = val * 10 + (uint64_t)dig[j];
        }
        if (!is_prime_uint64(val)) return false;
    }
    return true;
}

/* ---------- Main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* the sequence is 1‑based */

    init_base_primes();

    bool *segment = (bool*)malloc(SEG_SIZE * sizeof(bool));
    if (!segment) return (seq_t)0;

    uint64_t low = 2;
    uint64_t prime_index = 0;
    uint64_t good_cnt   = 0;

    while (1) {
        uint64_t high = low + SEG_SIZE - 1;
        uint64_t sqrt_high = ceil_sqrt_uint64(high);
        if (sqrt_high > BASE_MAX) extend_base_primes(sqrt_high);

        /* mark composites */
        memset(segment, 1, SEG_SIZE * sizeof(bool));
        if (low == 0) segment[0] = false;
        if (low == 1) segment[1] = false;

        for (size_t i = 0; i < base_count; ++i) {
            uint32_t p = base_primes[i];
            if ((uint64_t)p * (uint64_t)p > high) break;
            uint64_t start = (low + p - 1) / p * p;
            if (start < (uint64_t)p * (uint64_t)p) start = (uint64_t)p * (uint64_t)p;
            for (uint64_t j = start; j <= high; j += p)
                segment[j - low] = false;
        }

        /* scan primes in this segment */
        for (uint64_t i = 0; i < SEG_SIZE; ++i) {
            uint64_t num = low + i;
            if (num < 2) continue;
            if (!segment[i]) continue;          /* not prime */

            ++prime_index;
            if (has_prime_with_deletion(num)) {
                ++good_cnt;
                if (good_cnt == (uint64_t)n) {
                    free(segment);
                    return (seq_t)prime_index;   /* index of the n‑th good prime */
                }
            }
        }

        low = high + 1;
    }
}
