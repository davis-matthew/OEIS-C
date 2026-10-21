/*
 * A074765  Number of composites that are a concatenation nk of n and k for
 *          values of k from 1 to n.
 * https://oeis.org/A074765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of composite numbers of the form n||k for k = 1..n
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* small primes up to 97 for fast composite detection */
static const uint32_t small_prime[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97
};
static const int SMALL_PRIME_CNT = 25;

/* Miller‑Rabin bases sufficient for 64‑bit */
static const uint64_t mr_bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL};
static const int MR_BASE_CNT = 5;

/* pre‑computed 10^d up to d=20 (fits 64‑bit for our use) */
static uint64_t pow10[21];
static bool pow10_inited = false;

/* memoisation of sequence values */
static idx_t cached_n = 0;          /* highest n computed, 0 means none yet */
static seq_t *cache_arr = NULL;     /* zero‑based, cache_arr[i] stores a(i+1) */

/* number of decimal digits of x (x > 0) */
static inline int num_digits(idx_t x)
{
    if (x < 10ULL) return 1;
    if (x < 100ULL) return 2;
    if (x < 1000ULL) return 3;
    if (x < 10000ULL) return 4;
    if (x < 100000ULL) return 5;
    if (x < 1000000ULL) return 6;
    if (x < 10000000ULL) return 7;
    if (x < 100000000ULL) return 8;
    if (x < 1000000000ULL) return 9;
    if (x < 10000000000ULL) return 10;
    if (x < 100000000000ULL) return 11;
    if (x < 1000000000000ULL) return 12;
    if (x < 10000000000000ULL) return 13;
    if (x < 100000000000000ULL) return 14;
    if (x < 1000000000000000ULL) return 15;
    if (x < 10000000000000000ULL) return 16;
    if (x < 100000000000000000ULL) return 17;
    return 18;                     /* for values up to 10^18 */
}

/* 128‑bit modular multiplication (avoids overflow) */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* 128‑bit modular exponentiation */
static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    while (exp) {
        if (exp & 1ULL)
            result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* deterministic Miller–Rabin test for 64‑bit integers */
static bool is_prime_64(uint64_t n)
{
    if (n < 2) return false;
    /* trial division by small primes up to 97 */
    for (int i = 0; i < SMALL_PRIME_CNT; ++i) {
        uint32_t p = small_prime[i];
        if (n % p == 0) return n == (uint64_t)p;
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    for (int i = 0; i < MR_BASE_CNT; ++i) {
        uint64_t a = mr_bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
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

/* initialises pow10 array */
static void init_pow10(void)
{
    if (pow10_inited) return;
    pow10_inited = true;
    pow10[0] = 1ULL;
    for (int i = 1; i <= 20; ++i)
        pow10[i] = pow10[i - 1] * 10ULL;
}

/* counts composite concatenations for a single n */
static seq_t count_composites(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        int d = num_digits(k);
        uint64_t cand = n * pow10[d] + k;
        /* fast composite check using small primes */
        bool composite = false;
        for (int i = 0; i < SMALL_PRIME_CNT; ++i) {
            uint32_t p = small_prime[i];
            if (cand % p == 0) {
                composite = (cand != (uint64_t)p);
                break;
            }
        }
        if (!composite) {
            /* if not ruled composite by small primes, use MR */
            if (!is_prime_64(cand)) composite = true;
        }
        if (composite) ++count;
    }
    return count;
}

/* Public function required by the exercise */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset handling */
    init_pow10();

    if (n <= cached_n) {
        return cache_arr[n - 1];
    }

    /* extend cache */
    size_t old_size = cached_n;
    size_t new_size = n;
    seq_t *new_arr = (seq_t *)realloc(cache_arr, new_size * sizeof(seq_t));
    if (!new_arr) abort();
    cache_arr = new_arr;

    /* compute new values */
    for (idx_t i = old_size + 1; i <= n; ++i) {
        cache_arr[i - 1] = count_composites(i);
    }
    cached_n = n;
    return cache_arr[n - 1];
}
