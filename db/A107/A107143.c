/*
 * A107143  Primes of the form 2x^2 + 19y^2.
 * https://oeis.org/A107143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   seq_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes of the form 2*x^2 + 19*y^2.
   Generates primes in ascending order and returns the n‑th such prime. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_isqrt.h"

/* ---------- Prime sieve for trial division ---------- */
static uint32_t *prime_small = NULL;   /* array of primes up to prime_small_cap */
static size_t     prime_small_cnt = 0;
static size_t     prime_small_cap = 0;
#define PRIME_SIEVE_LIMIT 1000000U  /* enough for trial division up to ~10^12 */

static void init_prime_small(void)
{
    if (prime_small) return;  /* already initialized */

    /* Simple Sieve of Eratosthenes up to PRIME_SIEVE_LIMIT */
    bool *is_composite = (bool *)malloc(PRIME_SIEVE_LIMIT + 1);
    if (!is_composite) abort();
    memset(is_composite, 0, PRIME_SIEVE_LIMIT + 1);

    for (uint32_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            for (uint32_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                is_composite[j] = true;
        }
    }

    /* Store primes */
    prime_small_cap = PRIME_SIEVE_LIMIT / 10;
    prime_small = (uint32_t *)malloc(prime_small_cap * sizeof(uint32_t));
    if (!prime_small) abort();

    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (prime_small_cnt == prime_small_cap) {
                prime_small_cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(prime_small,
                                                    prime_small_cap * sizeof(uint32_t));
                if (!tmp) abort();
                prime_small = tmp;
            }
            prime_small[prime_small_cnt++] = i;
        }
    }
    free(is_composite);
}

/* ---------- Modular exponentiation for 64‑bit integers ---------- */
static seq_t modpow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1ULL;
    uint64_t base = a % mod;
    while (e) {
        if (e & 1ULL) res = (unsigned __int128)res * base % mod;
        base = (unsigned __int128)base * base % mod;
        e >>= 1ULL;
    }
    return (seq_t)res;
}

/* ---------- Deterministic Miller–Rabin for 64‑bit integers ---------- */
static bool miller_rabin_64(seq_t n)
{
    uint64_t nn = (uint64_t)n;
    if (nn < 2ULL) return false;
    if (nn % 2ULL == 0ULL) return nn == 2ULL;

    uint64_t d = nn - 1ULL;
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1ULL; ++s; }

    static const uint64_t bases[] =
        {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % nn == 0ULL) continue;
        seq_t x = modpow64(a, d, nn);
        if (x == 1 || x == (seq_t)(nn - 1)) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = modpow64((uint64_t)x, 2, nn);
            if (x == (seq_t)(nn - 1)) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- Primality test using trial division + MR ---------- */
static bool isprime_seq(seq_t n)
{
    if (n < 2) return false;

    init_prime_small();

    for (size_t i = 0; i < prime_small_cnt; ++i) {
        uint32_t p = prime_small[i];
        if ((seq_t)p * (seq_t)p > n) break;
        if (n % p == 0) return n == (seq_t)p;
    }
    /* If n is small enough, the above suffices */
    if (n <= (seq_t)prime_small[prime_small_cnt - 1] *
              (seq_t)prime_small[prime_small_cnt - 1])
        return true;

    /* otherwise use deterministic Miller–Rabin for 64‑bit */
    return miller_rabin_64(n);
}

/* ---------- Cache of primes of the desired form ---------- */
static seq_t *prime_list   = NULL;
static size_t prime_list_cap = 0;
static size_t prime_list_used = 0;
static seq_t next_candidate = 2;

/* Ensure that prime_list contains at least n primes. */
static void ensure_prime_list(idx_t n)
{
    while (prime_list_used < (size_t)n) {
        if (isprime_seq(next_candidate)) {
            /* test if next_candidate == 2*x^2 + 19*y^2 */
            seq_t y_max = seq_isqrt(next_candidate / (seq_t)19);
            bool ok = false;
            for (seq_t y = 0; y <= y_max; ++y) {
                seq_t py = (seq_t)19 * y * y;
                seq_t rem = next_candidate - py;
                if ((rem & (seq_t)1) != 0) continue;  /* must be even */
                seq_t half = rem / (seq_t)2;
                seq_t x = seq_isqrt(half);
                if (x * x == half) { ok = true; break; }
            }
            if (ok) {
                if (prime_list_used == prime_list_cap) {
                    size_t new_cap = prime_list_cap == 0 ? 64 : prime_list_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    prime_list = tmp;
                    prime_list_cap = new_cap;
                }
                prime_list[prime_list_used++] = next_candidate;
            }
        }
        ++next_candidate;
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; below 1 is irrelevant */
    ensure_prime_list(n);
    return prime_list[n - 1];
}
