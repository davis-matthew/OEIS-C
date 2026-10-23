/*
 * A136355  Numbers of the form P = product of the first k odd primes where
 *          P+2 is composite.
 * https://oeis.org/A136355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th product of the first k odd primes such that P+2
 *            is composite. The function computes P for each k in order, tests
 *            P+2 for compositeness, and returns the k-th qualifying product.
 *            It memoises results for arbitrary call ordering. For values that
 *            fit in a 64‑bit unsigned integer a dete...
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t); */

/* ---------  64‑bit deterministic primality test  --------- */

static uint64_t mod_mul_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result = mod_mul_u64(result, base, mod);
        base = mod_mul_u64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Miller–Rabin for 64‑bit integers, deterministic for all n < 2^64.
   Uses the set of bases {2, 325, 9375, 28178, 450775, 9780504, 1795265022}. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37,0};
    const uint64_t *p = small_primes;
    while (*p) {
        if (n % *p == 0)
            return n == *p;
        ++p;
    }

    /* write n-1 as d*2^s */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL, 0ULL};

    const uint64_t *b = bases;
    while (*b) {
        uint64_t a = *b % n;
        if (a == 0) {
            ++b;
            continue;
        }
        uint64_t x = mod_pow_u64(a, d, n);
        if (x == 1 || x == n - 1) {
            ++b;
            continue;
        }
        bool cont = false;
        for (unsigned r = 1; r < s; ++r) {
            x = mod_mul_u64(x, x, n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (!cont)
            return false;
        ++b;
    }
    return true;
}

/* Generic primality test for any seq_t.  Uses a fast 64‑bit test when the type
   is at most 64 bits; otherwise falls back on the supplied is_prime_big. */
static bool is_prime_seq(seq_t x)
{
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        uint64_t n = (uint64_t)x;
        return is_prime_uint64(n);
    } else {
        return is_prime_big(x);
    }
}

/* ---------  prime generation for odd primes  --------- */

static uint64_t *primes = NULL;     /* odd primes only */
static size_t   primes_cap = 0;
static size_t   primes_cnt = 0;

static void ensure_primes_cap(size_t need)
{
    if (primes_cap >= need)
        return;
    size_t new_cap = primes_cap ? primes_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    primes = tmp;
    primes_cap = new_cap;
}

static void add_next_prime(void)
{
    ensure_primes_cap(primes_cnt + 1);
    uint64_t cand = (primes_cnt == 0) ? 3 : primes[primes_cnt - 1] + 2;
    for (;;) {
        if (is_prime_seq((seq_t)cand)) {
            primes[primes_cnt++] = cand;
            break;
        }
        cand += 2;
    }
}

/* ---------  sequence memoisation  --------- */

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------  main entry function  --------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n<1 */

    size_t need = (size_t)n;
    ensure_cache_cap(need);

    /* State retained across calls */
    static seq_t current_product = (seq_t)1;  /* product of primes processed so far */
    static size_t next_prime_idx   = 0;      /* next prime index to multiply */

    /* Compute until we have at least `need` terms stored */
    while (cache_len < need) {
        /* Ensure we have the next odd prime available */
        if (next_prime_idx >= primes_cnt)
            add_next_prime();

        /* Multiply by that prime */
        current_product *= (seq_t)primes[next_prime_idx];

        /* Record if P+2 is composite */
        if (!is_prime_seq(current_product + (seq_t)2))
            cache[cache_len++] = current_product;

        ++next_prime_idx;
    }

    return cache[n - 1];
}
