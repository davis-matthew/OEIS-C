/*
 * A095081  Fibodd primes, i.e., primes p whose Zeckendorf-expansion
 *          A014417(p) ends with one.
 * https://oeis.org/A095081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 2530, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9749 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A095081: primes p whose Zeckendorf expansion A014417(p) ends with 1.
   The function computes primes by a sieve-like search and caches the
   results so that any n ≥ 1 can be answered quickly. */

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

/* --------------------------------------------------------------------- */
static bool has_zeckendorf_ending_one(seq_t p) {
    /* Dynamic array of Fibonacci numbers (F1 = 1, F2 = 2, Fk = Fk-1+Fk-2). */
    static seq_t *fibs = NULL;
    static size_t fib_len = 0;   /* number of terms stored */
    static size_t fib_cap = 0;   /* allocated capacity */

    /* Initialise the table on first use. */
    if (fib_len == 0) {
        fib_cap = 8;
        fibs = mem_new(seq_t, fib_cap);
        if (!fibs) return false;   /* allocation failed – treat as false */
        fibs[0] = 1;   /* F1 */
        fibs[1] = 2;   /* F2 */
        fib_len = 2;
    }

    /* Grow the table until the largest Fibonacci number exceeds p. */
    while (fibs[fib_len - 1] <= p) {
        if (fib_len == fib_cap) {
            size_t new_cap = fib_cap * 2;
            seq_t *tmp = mem_resize(seq_t, fibs, new_cap);
            if (!tmp) return false;   /* allocation failed – treat as false */
            fibs = tmp;
            fib_cap = new_cap;
        }
        fibs[fib_len] = fibs[fib_len - 1] + fibs[fib_len - 2];
        ++fib_len;
    }

    seq_t remain = p;
    idx_t last_used_index = 0;   /* 1‑based index of the smallest Fibonacci used */

    /* Greedy decomposition (Zeckendorf theorem). */
    for (idx_t i = fib_len; i-- > 0; ) {
        if (remain == 0) break;
        if (remain >= fibs[i]) {
            remain -= fibs[i];
            last_used_index = i + 1;   /* i is zero‑based, but indices are 1‑based. */
        }
    }

    return (remain == 0) && (last_used_index == 1);
}

/* --------------------------------------------------------------------- */
/* Helper: Zeckendorf representation test                                   */

/* --------------------------------------------------------------------- */
/* Main sequence function                                                   */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset is 1; undefined for n = 0 */

    /* Cache of found Fibodd primes. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of stored primes */
    static size_t cache_cap = 0;   /* allocated capacity */

    if (cache == NULL) {
        cache_cap = 32;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return 0;   /* allocation failed – return 0 */
    }

    /* Generate new primes until we have at least n entries. */
    while (cache_len < n) {
        seq_t candidate = 2;
        if (cache_len > 0) candidate = cache[cache_len - 1] + 1;  /* start after last prime */

        /* Search for the next Fibodd prime. */
        for (;;) {
            if (candidate < 2) candidate = 2;  /* safety */

            if (is_prime_big(candidate) && has_zeckendorf_ending_one(candidate)) {
                /* Store the new prime. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) return 0;  /* allocation failed – return 0 */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = candidate;
                break;   /* move on to generate the next prime if needed */
            }
            candidate = candidate + 1;
        }
    }

    return cache[n - 1];
}
