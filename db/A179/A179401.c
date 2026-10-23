/*
 * A179401  Numbers k such that phi(phi(k)) = lambda(lambda(k)).
 * https://oeis.org/A179401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 791, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A179401: Numbers k such that φ(φ(k)) = λ(λ(k)).

   The function a(idx_t n) finds the n-th such number by scanning k
   in increasing order, computing φ(k) and λ(k) using
   64‑bit arithmetic, and storing the results in a static array.
   No external utilities are required beyond the standard headers.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- prime sieve for factorisation ---------- */
#define PRIME_LIMIT 1000000   /* should cover √(typical k) for first 10k terms */

static size_t prime_count = 0;
static uint32_t *primes = NULL;

static void build_primes(void)
{
    static bool built = false;
    if (built) return;
    built = true;
    bool *is_comp = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_comp) return;
    primes = (uint32_t*)malloc(PRIME_LIMIT * sizeof(uint32_t));
    if (!primes) { free(is_comp); return; }

    size_t cnt = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    prime_count = cnt;
    free(is_comp);
}

/* ---------- GCD ---------- */
static uint64_t uint64_gcd(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* ---------- Euler's totient φ(n) for 64-bit n ---------- */
static uint64_t phi_uint64(uint64_t n)
{
    if (n == 1) return 1;
    uint64_t result = n;
    uint64_t m = n;
    build_primes();
    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * primes[i] <= m; ++i) {
        uint32_t p = primes[i];
        if (m % p == 0) {
            result = result / p * (p - 1);
            do { m /= p; } while (m % p == 0);
        }
    }
    if (m > 1) { /* remaining prime factor */
        result = result / m * (m - 1);
    }
    return result;
}

/* ---------- Carmichael λ(n) for 64-bit n ---------- */
static uint64_t lambda_uint64(uint64_t n)
{
    if (n == 1) return 1;
    uint64_t result = 1;
    uint64_t m = n;
    build_primes();
    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * primes[i] <= m; ++i) {
        uint32_t p = primes[i];
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) { m /= p; ++exp; }

            uint64_t lam;
            if (p == 2) {
                if (exp == 1) lam = 1;
                else if (exp == 2) lam = 2;
                else lam = (uint64_t)1 << (exp - 2);  /* 2^(exp-2) */
            } else {
                /* λ(p^exp) = p^(exp-1)*(p-1) */
                uint64_t pow = 1;
                for (int k = 0; k < exp - 1; ++k) pow *= p;
                lam = pow * (p - 1);
            }

            uint64_t g = uint64_gcd(result, lam);
            result = result / g * lam;
        }
    }
    if (m > 1) { /* remaining prime factor */
        uint64_t p = m;
        uint64_t lam = (p == 2) ? 1 : p - 1;
        uint64_t g = uint64_gcd(result, lam);
        result = result / g * lam;
    }
    return result;
}

/* ---------- dynamic array for the sequence ---------- */
static seq_t *seq_cache = NULL;
static idx_t seq_len   = 0;
static size_t seq_cap  = 0;

/* ---------- ensure capacity ---------- */
static void seq_ensure(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap * 2 : 64;
    while (newcap < needed) newcap *= 2;
    seq_t *tmp = (seq_t*)realloc(seq_cache, newcap * sizeof(seq_t));
    if (!tmp) return; /* allocation failed, keep old */
    seq_cache = tmp;
    seq_cap = newcap;
}

/* ---------- the main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but guard */
    /* generate terms until having at least n terms */
    static uint64_t next_candidate = 1;
    while (seq_len < n) {
        uint64_t k = next_candidate++;
        uint64_t phi1 = phi_uint64(k);
        uint64_t phi2 = phi_uint64(phi1);
        uint64_t lam1 = lambda_uint64(k);
        uint64_t lam2 = lambda_uint64(lam1);
        if (phi2 == lam2) {
            seq_ensure(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)k;
        }
    }
    return seq_cache[n - 1];
}
