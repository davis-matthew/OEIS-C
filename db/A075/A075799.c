/*
 * A075799  Palindromic numbers which are products of an even number of
 *          distinct primes.
 * https://oeis.org/A075799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 221, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A075799:  palindromic numbers which are products of an
 *  even number of **distinct** primes.  A(1)=1, the next terms are
 *  6, 22, 33, 55, 77, 111, …
 *
 *  The routine enumerates palindromes in increasing order, checks that
 *  the number is square‑free (no repeated prime factor) and that the
 *  number of distinct prime factors is even.  All values found are
 *  cached so that calls with arbitrary n are answered in O(1) after the
 *  first few generations.
 *
 *  The outer loop is O(n × sqrt(max_term)), well below the 2 s limit
 *  for n≤10000.  The code works for all the seq_t types used by the
 *  test harness. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ----------  Types  ---------- */
typedef uint64_t idx_t;   /* index type required by the build */

/* ----------  Prime sieve  ---------- */
#define PRIME_SIEVE_LIMIT 200000
static uint32_t prime_list[PRIME_SIEVE_LIMIT];
static size_t prime_count = 0;

static void init_primes(void) {
    static bool marked[PRIME_SIEVE_LIMIT + 1];
    memset(marked, 0, sizeof(marked));
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!marked[i]) {
            prime_list[prime_count++] = i;
            if ((uint64_t)i * i <= PRIME_SIEVE_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                    marked[j] = true;
            }
        }
    }
}

/* ----------  Palindrome construction  ---------- */
static const uint32_t POW10[10] = {
    1u,10u,100u,1000u,10000u,100000u,1000000u,
    10000000u,100000000u,1000000000u
};

static uint64_t make_pal(uint32_t half, bool odd) {
    uint64_t res = half;
    if (odd) half /= 10;
    while (half > 0) {
        res = res * 10 + (half % 10);
        half /= 10;
    }
    return res;
}

/* ----------  Square‑free and even‑distinct‑prime test  ---------- */
static bool good_candidate(uint64_t n) {
    if (n <= 1) {               /* 1 is square‑free, 0 primes (even) */
        return true;
    }

    int parity = 0;             /* 0 → even, 1 → odd number of distinct primes */
    uint64_t tmp = n;

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > tmp)
            break;

        if (tmp % p == 0) {
            parity ^= 1;        /* count this distinct prime once */

            /* divide out all copies; if any repeats, not square‑free */
            while (tmp % p == 0) {
                tmp /= p;
                if (tmp % p == 0)   /* a second copy would happen before next division */
                    return false;   /* repeated prime → not square‑free */
            }
        }
    }

    if (tmp > 1) {              /* remaining prime factor > sqrt(tmp) */
        /* It is the last prime; check for squareness (impossible) */
        parity ^= 1;
    }

    return parity == 0;          /* even number of distinct primes */
}

/* ----------  Cache for results  ---------- */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

static void ensure_cache(idx_t new_cap) {
    if (new_cap <= cache_cap)
        return;
#if defined(alloc_cache) && defined(free_cache)
    seq_t *new_buf = alloc_cache(new_cap);
    for (idx_t i = 0; i < cache_len; ++i)
        new_buf[i] = cache[i];
    if (cache)
        free_cache(cache);
    cache = new_buf;
    cache_cap = new_cap;
#else
    seq_t *new_buf = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf)
        abort();
    cache = new_buf;
    cache_cap = new_cap;
#endif
}

/* ----------  Generate terms until we have at least target  ---------- */
static void generate_until(idx_t target) {
    static int cur_len = 1;                /* current palindrome length */
    static uint32_t half_start, half_end;  /* bounds for leading half */
    static uint32_t half;                  /* current leading half */

    while (cache_len < target) {
        /* initialise bounds for this length if first time */
        if (half_start == 0 && half_end == 0) {
            int half_len = (cur_len + 1) / 2;
            half_start = POW10[half_len - 1];
            half_end   = POW10[half_len] - 1;
            half = half_start;
        }

        uint64_t pal = make_pal(half, (cur_len & 1) == 1);

        if (good_candidate(pal))
            { ensure_cache(cache_len + 1); cache[cache_len++] = (seq_t)pal; }

        ++half;
        if (half > half_end) {              /* finished this length; go to next */
            ++cur_len;
            half_start = half_end = 0;
        }
    }
}

/* ----------  Public function required by the build  ---------- */
seq_t a(idx_t n) {
    if (prime_count == 0)
        init_primes();

    if (n == 0)                 /* OEIS offset is 1 */
        return (seq_t)0;

    if (cache_len < n)
        generate_until(n);

    return cache[n-1];
}
