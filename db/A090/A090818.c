/*
 * A090818  Nonprime numbers k such that 3*(3^k-1)*B(2*k) is an integer, where
 *          B(k) denotes the k-th Bernoulli number.
 * https://oeis.org/A090818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A090818:
 *   Non‑prime integers k such that 3*(3^k-1)*B(2k) is an integer.
 *   Equivalently, for every prime p > 3 with p‑1 | 2k, we must have
 *   3^k ≡ 1 (mod p).  The implementation precomputes all such k up
 *   to a fixed limit and stores the non‑prime ones in a static array.
 *   a(idx_t n) returns the n‑th element of this array (1‑based).
 *   The routine is O(K log log K) with K = 200000.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Constants ---------------------------------------------------- */
#define MAX_K        200000UL            /* maximum k to precompute */
#define SIEVE_LIMIT  (2 * MAX_K + 16UL)  /* primes needed up to 2k+1 */

/* Static storage ------------------------------------------------- */
static uint8_t  *is_prime   = NULL;  /* sieve (0/1) */
static int     *prime_list = NULL;  /* list of primes */
static size_t    num_primes = 0;
static uint8_t  *valid_k    = NULL;  /* validity flags for k */
static seq_t    *seq_vals   = NULL;  /* array of sequence values */
static size_t    seq_len    = 0;
static bool      built      = false;

/* ----------------------------------------------------------------*/
/* Modular exponentiation for uint64_t modulus (uses 128‑bit).
 * Returns (base^exp) % mod. */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1;
    }
    return result;
}

/* ----------------------------------------------------------------*/
/* Compute the multiplicative order of 3 modulo a prime p (p>3). */
static uint64_t order_of_3_mod_p(uint64_t p)
{
    uint64_t phi = p - 1;
    uint64_t order = phi;
    uint64_t temp = phi;

    for (size_t i = 0; i < num_primes &&
         (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= temp; ++i) {
        uint64_t q = (uint64_t)prime_list[i];
        if (temp % q == 0) {
            while (temp % q == 0) temp /= q;
            while (order % q == 0 && mod_pow_uint64(3ULL, order / q, p) == 1ULL)
                order /= q;
        }
    }
    if (temp > 1) {                 /* remaining prime factor */
        uint64_t q = temp;
        while (order % q == 0 && mod_pow_uint64(3ULL, order / q, p) == 1ULL)
            order /= q;
    }
    return order;
}

/* ----------------------------------------------------------------*/
/* Build primes up to SIEVE_LIMIT and the sequence. */
static void build_sequence(void)
{
    /* 1. Sieve of Eratosthenes */
    is_prime = (uint8_t*)malloc(SIEVE_LIMIT + 1);
    for (uint64_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (uint64_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = 0;

    /* 2. Extract primes into array */
    for (uint64_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i]) ++num_primes;
    prime_list = (int*)malloc(num_primes * sizeof(int));
    size_t idx = 0;
    for (uint64_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i]) prime_list[idx++] = (int)i;

    /* 3. Validity array for k */
    valid_k = (uint8_t*)malloc((MAX_K + 1) * sizeof(uint8_t));
    for (size_t i = 0; i <= MAX_K; ++i) valid_k[i] = 1;    /* assume true */

    /* 4. For every prime p>3, mark invalid k */
    for (size_t pi = 0; pi < num_primes; ++pi) {
        int p_int = prime_list[pi];
        if (p_int <= 3) continue;                  /* p=2,3 are ignored */
        uint64_t d = (uint64_t)(p_int - 1) / 2ULL;  /*  (p-1)/2 */
        if (d > MAX_K) continue;
        uint64_t ord = order_of_3_mod_p((uint64_t)p_int);
        for (uint64_t k = d; k <= MAX_K; k += d)
            if (k % ord != 0ULL)
                valid_k[k] = 0;                     /* invalid */
    }

    /* 5. Gather non‑prime valid k into seq_vals */
    size_t capacity = 1024;
    seq_vals = (seq_t*)malloc(capacity * sizeof(seq_t));
    seq_len = 0;
    for (uint64_t k = 1; k <= MAX_K; ++k) {
        if (!valid_k[k]) continue;
        if (k <= SIEVE_LIMIT && is_prime[k]) continue;  /* skip primes */
        if (seq_len == capacity) {
            capacity *= 2;
            seq_vals = (seq_t*)realloc(seq_vals, capacity * sizeof(seq_t));
        }
        seq_vals[seq_len++] = (seq_t)k;
    }

    built = true;
}

/* ----------------------------------------------------------------*/
/* Public sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!built) build_sequence();
    if (n < 1 || n > seq_len) return 0;
    return seq_vals[(size_t)(n - 1)];
}
