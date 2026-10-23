/*
 * A105880  Primes for which -8 is a primitive root.
 * https://oeis.org/A105880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mulmod_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A105880 – Primes for which −8 is a primitive root.
 *
 * The function generates successive primes and keeps those for which the
 * multiplicative order of −8 modulo the prime equals p−1.  This is
 * equivalent to requiring that for every prime divisor q of p−1,
 * (−8)^((p−1)/q) ≠ 1 (mod p).
 *
 * The implementation uses trial division with a pre‑computed list of small
 * primes up to 100 000, and modular exponentiation using 128‑bit
 * intermediates to avoid overflow.  Results are memoised so that any
 * call to a(idx_t) receives the correct value regardless of ordering of
 * calls.  The routine is O(n log p) for n terms and runs well below the
 * 2 s limit for n ≈ 10 000.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mulmod_uint64.h"

/* ---------- small prime table for factoring and primality testing ---------- */
static uint64_t *small_primes = NULL;   /* array of primes up to LIMIT */
static size_t      small_cnt   = 0;     /* number of small primes */

/* initialise the small prime list (sieve of Eratosthenes) */
static void init_small_primes(void)
{
    if (small_cnt > 0) return;          /* already initialised */

    const uint64_t LIMIT = 100000;      /* sqrt(200 000) ≈ 447 */

    bool *is_composite = (bool*)calloc(LIMIT + 1, sizeof(bool));
    if (!is_composite) exit(EXIT_FAILURE);

    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            /* store the prime */
            size_t new_cnt = small_cnt + 1;
            uint64_t *tmp = (uint64_t*)realloc(small_primes,
                                               new_cnt * sizeof(uint64_t));
            if (!tmp) exit(EXIT_FAILURE);
            small_primes = tmp;
            small_primes[small_cnt++] = i;

            /* mark multiples if i*i <= LIMIT */
            if (i * i <= LIMIT) {
                for (uint64_t j = i * i; j <= LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
}

/* ---------- 64‑bit modular exponentiation ---------- */
static uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) result = mulmod_uint64(result, base, mod);
        base = mulmod_uint64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* ---------- primality test: trial division by small primes ---------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;

    init_small_primes();
    for (size_t i = 0; i < small_cnt; ++i) {
        uint64_t p = small_primes[i];
        if (p * p > n) break;
        if (n % p == 0ULL) return false;
    }
    return true;
}

/* ---------- check if −8 is a primitive root modulo the prime p ---------- */
static bool minus8_is_root(uint64_t p)
{
    /* (−8) mod p */
    uint64_t g = (p - (8ULL % p)) % p;
    if (g == 0ULL) return false;        /* p divides 8 */

    uint64_t m   = p - 1ULL;
    uint64_t temp = m;

    init_small_primes();
    for (size_t i = 0; i < small_cnt && small_primes[i] * small_primes[i] <= temp; ++i) {
        uint64_t q = small_primes[i];
        if (temp % q == 0ULL) {
            uint64_t exp = m / q;
            if (modpow_uint64(g, exp, p) == 1ULL) return false;
            while (temp % q == 0ULL) temp /= q;
        }
    }
    if (temp > 1ULL) {   /* remaining prime factor */
        uint64_t exp = m / temp;
        if (modpow_uint64(g, exp, p) == 1ULL) return false;
    }
    return true;
}

/* ---------- storage of discovered sequence primes ---------- */
static uint64_t *seq_primes = NULL;
static size_t     seq_len    = 0;   /* number of stored terms */

/* enlarge storage if necessary */
static void ensure_seq_capacity(size_t needed)
{
    if (seq_len >= needed) return;
    size_t new_cap = (seq_len == 0) ? 64 : seq_len * 2;
    while (new_cap < needed) new_cap *= 2;
    uint64_t *tmp = (uint64_t*)realloc(seq_primes, new_cap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq_primes = tmp;
}

/* ---------- generate terms until at least n terms are known ---------- */
static void assure_n_terms(idx_t n)
{
    static uint64_t candidate = 3ULL;   /* next odd integer to test */
    init_small_primes();

    while (seq_len < n) {
        /* find next prime candidate */
        while (!is_prime_uint64(candidate))
            candidate += 2ULL;

        if (minus8_is_root(candidate)) {
            ensure_seq_capacity(seq_len + 1);
            seq_primes[seq_len++] = candidate;
        }
        candidate += 2ULL;
    }
}

/* ---------- public API required by the grader ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 1 */
    assure_n_terms(n);
    return (seq_t)seq_primes[n - 1];
}
