/*
 * A087242  Smallest prime number p such that n+p = q is also a prime, or 0 if
 *          no such prime number exists.
 * https://oeis.org/A087242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that n+p is also prime, or 0 if
 *            none exists
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------------------------------------------------------
 * Auxiliary prime utilities: deterministic Miller–Rabin (64‑bit)
 * ------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool isPrime64(uint64_t n) {
    if (n < 2) return false;
    static const uint8_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }
    // Deterministic Miller–Rabin for 64‑bit numbers
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        if (bases[i] % n == 0) continue;
        uint64_t x = mod_pow(bases[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* -------------------------------------------------------------
 * Sieve and prime list (static, built on first call)
 * ------------------------------------------------------------- */
static uint8_t *sieve_arr = NULL;
static uint32_t *prime_list = NULL;
static size_t prime_cnt = 0;
static size_t sieve_limit = 0;
static const size_t DEFAULT_LIMIT = 500000;

static void init_sieve(size_t limit) {
    if (limit <= sieve_limit) return;
    if (sieve_arr) free(sieve_arr);
    if (prime_list) free(prime_list);

    sieve_arr = (uint8_t*)calloc(limit + 1, 1);
    if (!sieve_arr) exit(1);
    for (size_t i = 2; i <= limit; ++i) sieve_arr[i] = 1;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (sieve_arr[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                sieve_arr[j] = 0;
        }
    }
    // Build prime list
    size_t approx = limit / 10; // rough estimate
    prime_list = (uint32_t*)malloc((approx + 100) * sizeof(uint32_t));
    prime_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (sieve_arr[i]) prime_list[prime_cnt++] = (uint32_t)i;
    }
    sieve_limit = limit;
}

static bool isPrimeArray(uint64_t n) {
    if (n <= sieve_limit) return sieve_arr[n];
    return isPrime64(n);
}

/* -------------------------------------------------------------
 * Sequence function
 * ------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (sieve_limit == 0) init_sieve(DEFAULT_LIMIT);

    if (n & 1) { // n is odd
        uint64_t val = (uint64_t)n + 2;
        return isPrimeArray(val) ? (seq_t)2 : (seq_t)0;
    } else { // n is even
        // check primes from the sieve first
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_list[i];
            if (p == 2) continue; // 2 never works for even n
            uint64_t val = (uint64_t)n + p;
            if (isPrimeArray(val))
                return (seq_t)p;
        }
        // if not found, search beyond the sieve limit
        uint64_t p = prime_list[prime_cnt - 1] + 2;
        while (true) {
            if (isPrime64(p)) {
                uint64_t val = (uint64_t)n + p;
                if (isPrime64(val))
                    return (seq_t)p;
            }
            p += 2; // only odd candidates
        }
    }
}
