/*
 * A056913  Odd squarefree numbers for which the number of prime divisors is
 *          even.
 * https://oeis.org/A056913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th odd squarefree integer with an even number of prime
 *            factors (including 1)
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/* --------- Helper: generate prime table up to a fixed limit --------- */
static uint64_t *primes = NULL;      /* array of primes */
static size_t   primes_cnt = 0;      /* number of primes stored */

/* precompute primes up to LIMIT using a simple sieve */
static void init_primes(void) {
    if (primes_cnt != 0)
        return;                      /* already initialized */

    const int LIMIT = 2000000;        /* sufficient for the needed range */
    char *is_comp = (char *)calloc(LIMIT + 1, 1);
    size_t cap = 200000;             /* initial capacity, will grow */
    primes = (uint64_t *)malloc(cap * sizeof(uint64_t));
    primes_cnt = 0;

    for (int i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            /* add prime i */
            if (primes_cnt == cap) {
                cap *= 2;
                primes = (uint64_t *)realloc(primes, cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = (uint64_t)i;

            if ((int64_t)i * i <= LIMIT) {
                for (int j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
}

/* --------- Helper: test whether a number is odd, square‑free and has an even number of prime factors --------- */
static bool is_valid(seq_t x)
{
    if ((x & (seq_t)1) == 0)
        return false;                /* must be odd */

    seq_t tmp = x;
    int prime_cnt = 0;

    /* trial division by pre‑computed primes */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t pp = (seq_t)p;
        if (pp * pp > tmp)
            break;

        if (tmp % pp == 0) {
            ++prime_cnt;
            tmp /= pp;
            /* check for a squared prime factor */
            if (tmp % pp == 0)
                return false;        /* not square‑free */
        }
    }

    if (tmp > (seq_t)1)
        ++prime_cnt;                 /* remaining prime factor */

    return (prime_cnt % 2) == 0;     /* even number of prime factors */
}

/* --------- Cache for computed terms --------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;          /* number of stored terms (index 0 unused) */
static size_t cache_cap = 0;
static seq_t  current_candidate = 1;  /* last checked integer */

/* ensure cache has capacity for at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < need)
            new_cap *= 2;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_ptr;
        cache_cap = new_cap;
    }
}

/* initialise static data structures */
static void init_sequence(void)
{
    init_primes();

    if (cache_len == 0) {
        ensure_cache_capacity(6);
        cache[0] = (seq_t)0;        /* dummy to make indices 1‑based */
        cache[1] = (seq_t)1;        /* first term */
        cache_len = 2;
        current_candidate = 1;
    }
}

/* ------------------------------------------------------------------ */
/* public interface required by the test harness                     */
seq_t a(idx_t n)
{
    assert(n >= 1);

    init_sequence();

    /* extend cache until we have computed term n */
    while (cache_len <= (size_t)n) {
        seq_t cand = current_candidate + 1;
        if ((cand & (seq_t)1) == 0)
            ++cand;                 /* next odd candidate */

        for (;; cand += 2) {        /* skip even numbers */
            if (is_valid(cand)) {
                ensure_cache_capacity(cache_len + 1);
                cache[cache_len++] = cand;
                current_candidate = cand;
                break;
            }
        }
    }

    return cache[(size_t)n];
}
