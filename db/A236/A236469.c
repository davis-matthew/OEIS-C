/*
 * A236469  Primes p such that pi(p) = floor(p/10), where pi is the prime
 *          counting function.
 * https://oeis.org/A236469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th prime p such that pi(p)=floor(p/10) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Deterministic Miller–Rabin for 64‑bit integers */
static uint64_t mulmod_u64(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static uint64_t modpow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    while (exp > 0) {
        if (exp & 1) result = mulmod_u64(result, base, mod);
        base = mulmod_u64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    size_t sp_len = sizeof small_primes / sizeof small_primes[0];
    for (size_t i = 0; i < sp_len; ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    size_t b_len = sizeof bases / sizeof bases[0];
    for (size_t i = 0; i < b_len; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow_u64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mulmod_u64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Cache of primes satisfying the condition */
static uint64_t *prime_cache = NULL;
static size_t pc_len      = 0;
static size_t pc_cap      = 0;

static void ensure_prime_cache_capacity(size_t needed)
{
    if (needed <= pc_cap) return;
    size_t new_cap = pc_cap ?: 16;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_cache = tmp;
    pc_cap = new_cap;
}

/* State used in the search */
static idx_t next_candidate = 2;   /* next integer to test */
static idx_t prime_cnt     = 0;     /* pi(p) for the found prime */

seq_t a(idx_t n)
{
    while (pc_len < (size_t)n) {
        /* Find the next prime */
        for (;;) {
            if (is_prime_u64((uint64_t)next_candidate)) {
                ++prime_cnt;                          /* this is prime_cnt-th prime */
                if (prime_cnt == next_candidate / 10) {
                    ensure_prime_cache_capacity(pc_len + 1);
                    prime_cache[pc_len++] = (uint64_t)next_candidate;
                }
                break;  /* prime found, exit inner loop */
            }
            ++next_candidate;
            if (next_candidate > 2 && (next_candidate & 1ULL) == 0)
                ++next_candidate;  /* skip even numbers */
        }
        ++next_candidate;  /* start next search from the following number */
    }
    return (seq_t)prime_cache[n - 1];
}
