/*
 * A192862  Flat primes: odd primes p such that p+1 is a squarefree number
 *          times a power of two.
 * https://oeis.org/A192862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth odd prime p such that p+1 is a square‐free * odd
 *            number times a power of two. (OEIS A192862)
 * Range:     u8 n <= 40, u16 n <= 4896, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* --- Prime generation ---------------------------------------------------- */
#define LIMIT      200000            /* enough for n up to ~10 000 */
#define MAX_PRIMES 20000

static idx_t prime_list[MAX_PRIMES];
static size_t prime_list_cnt = 0;
static bool initialized_primes = false;

static void init_primes(void)
{
    static bool marked[LIMIT + 1] = {false};
    if (initialized_primes) return;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!marked[i]) {
            if (prime_list_cnt < MAX_PRIMES)
                prime_list[prime_list_cnt++] = i;
            if (i * i <= LIMIT) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    marked[j] = true;
            }
        }
    }
    initialized_primes = true;
}

/* --- Utility tests ----------------------------------------------------- */
static bool is_prime_idx(idx_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_list_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

static bool is_flat_prime_idx(idx_t p)
{
    /* p is an odd prime */
    idx_t m = p + 1;
    while ((m & 1) == 0) m >>= 1;      /* remove the power of two */
    idx_t tmp = m;

    for (size_t i = 0; i < prime_list_cnt; ++i) {
        idx_t r = prime_list[i];
        if (r * r > tmp) break;
        if (tmp % r == 0) {
            int exp = 0;
            do {
                tmp /= r;
                ++exp;
            } while (tmp % r == 0);
            if (exp > 1) return false; /* not square‑free */
        }
    }
    return true;
}

/* --- Cache of computed terms ------------------------------------------ */
static seq_t *cache = NULL;
static size_t cache_cnt = 0;
static size_t cache_cap = 0;
static idx_t candidate = 3;                  /* next odd number to test */

static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                      /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* --- Main sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */
    init_primes();

    /* Build cache up to n terms */
    while (cache_cnt < (size_t)n) {
        idx_t cand = candidate;
        if (is_prime_idx(cand) && is_flat_prime_idx(cand)) {
            if (cache_cnt == cache_cap) grow_cache();
            cache[cache_cnt++] = (seq_t)cand;
        }
        candidate += 2;                  /* next odd candidate */
    }
    return cache[(size_t)n - 1];
}
