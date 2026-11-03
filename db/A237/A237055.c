/*
 * A237055  Least prime p not already included in the sequence such that p +
 *          2*n is prime.
 * https://oeis.org/A237055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least unused prime p such that p + 2*n is prime
 * Range:     u8 n <= 37, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Static state for the sequence, shared across calls                 */
/* ------------------------------------------------------------------ */
static seq_t *seq_cache = NULL;   /* cache of computed a(k) (k≥1) in zero‑based index */
static size_t cache_len  = 0;     /* number of terms stored in seq_cache          */
static size_t cache_cap  = 0;     /* allocation capacity of seq_cache            */

static uint64_t *prime_list = NULL;   /* list of all primes found so far           */
static size_t primes_len  = 0;        /* number of primes in prime_list            */
static size_t primes_cap  = 0;        /* allocation capacity of prime_list          */
static bool   *prime_used  = NULL;    /* true if prime_list[i] already used in seq */
static uint64_t next_prime_candidate = 2;  /* integer to try for the next prime */

/* ------------------------------------------------------------------ */
/* Helper to grow the prime list                                      */
/* ------------------------------------------------------------------ */
static void add_next_prime(void)
{
    /* grow containers if necessary */
    if (primes_len >= primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 8;
        uint64_t *new_primes = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
        bool     *new_used   = (bool*)realloc(prime_used,   new_cap * sizeof(bool));
        if (!new_primes || !new_used) abort();
        prime_list = new_primes;
        prime_used = new_used;
        primes_cap = new_cap;
    }

    /* find next prime (smallest integer ≥ next_prime_candidate that is prime) */
    while (!is_prime_big(next_prime_candidate))
        ++next_prime_candidate;

    prime_list[primes_len]     = next_prime_candidate;
    prime_used[primes_len]     = false;      /* prime not yet used in sequence */
    ++primes_len;
    ++next_prime_candidate;
}

/* ------------------------------------------------------------------ */
/* Ensure that seq_cache contains terms up to a(n) (n≥1)              */
/* ------------------------------------------------------------------ */
static void ensure_cache_len(size_t n)
{
    if (n <= cache_len) return; /* already computed */

    /* compute additional terms up to n */
    for (size_t cur = cache_len + 1; cur <= n; ++cur) {
        /* offset = 2 * cur, cast to seq_t for arithmetic with seq_t values */
        const seq_t offset = (seq_t)cur + (seq_t)cur;

        size_t pi = 0;
        for (;;) {
            /* make sure we have enough primes */
            if (pi >= primes_len) add_next_prime();

            uint64_t p_val = prime_list[pi];
            if (!prime_used[pi]) {
                /* test primality of p + 2*cur */
                const seq_t cand = (seq_t)p_val;
                const seq_t sum  = cand + offset;
                if (is_prime_big(sum)) {
                    /* found the next term */
                    if (cache_len + 1 > cache_cap) {
                        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                        seq_t *new_cache = mem_resize(seq_t, seq_cache, new_cap);
                        if (!new_cache) abort();
                        seq_cache = new_cache;
                        cache_cap = new_cap;
                    }
                    seq_cache[cache_len] = cand;
                    ++cache_len;
                    prime_used[pi] = true;
                    break;
                }
            }
            ++pi;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public API required by the framework                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1; undefined for n=0, but harmless */
    ensure_cache_len((size_t)n);
    return seq_cache[n - 1]; /* seq_cache is zero‑based: a(1) -> index 0 */
}
