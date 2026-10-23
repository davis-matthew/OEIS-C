/*
 * A107918  Length of continued fraction for the harmonic mean of n and n-th
 *          prime.
 * https://oeis.org/A107918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the continued fraction expansion of (2 · n ·
 *            p_n) / (n + p_n), where p_n is the n‑th prime
 * Range:     u8 n <= 10, u16 n <= 81, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* --------- Miller–Rabin primality test for 64‑bit integers --------- */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

static inline uint64_t pow_mod(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t r = 1;
    while (d != 0) {
        if (d & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        d >>= 1;
    }
    return r;
}

static bool is_prime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] =
        {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* --------- Prime caching (dynamic array) --------- */
static uint64_t *prime_cache = NULL;
static size_t prime_cap = 0;
static size_t prime_cnt = 0;
static uint64_t last_checked = 1;

static void ensure_primes(size_t needed)
{
    while (prime_cnt < needed) {
        ++last_checked;
        if (is_prime64(last_checked)) {
            if (prime_cnt >= prime_cap) {
                size_t new_cap = prime_cap ? prime_cap << 1 : 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_cache,
                                                    new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                prime_cache = tmp;
                prime_cap = new_cap;
            }
            prime_cache[prime_cnt++] = last_checked;
        }
    }
}

static inline uint64_t get_nth_prime(size_t n)
{
    ensure_primes(n);
    return prime_cache[n - 1];
}

/* --------- GCD for seq_t (implemented for all integer types) --------- */
static inline seq_t my_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* --------- Main sequence function ------------------------------------ */
seq_t a(idx_t n)
{
    /* Obtain the n-th prime number (n ≥ 1) */
    uint64_t p = get_nth_prime((size_t)n);

    /* Harmonic mean: numerator = 2·n·p, denominator = n + p */
    seq_t num = (seq_t)2 * (seq_t)n * (seq_t)p;
    seq_t den = (seq_t)n + (seq_t)p;

    /* Reduce the fraction (helps keep intermediate sizes small) */
    seq_t g = my_gcd(num, den);
    if (g > 1) {
        num /= g;
        den /= g;
    }

    /* Count terms of the simple continued fraction expansion */
    seq_t len = 0;
    while (den != 0) {
        seq_t r = num % den;
        num = den;
        den = r;
        ++len;
    }

    return len;
}
