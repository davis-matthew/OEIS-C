/*
 * A155847  Primes of the form 2^p-p*q where p is prime,q=7.
 * https://oeis.org/A155847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 4, i64 n <= 4, i128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A155847: primes of the form 2^p – 7p where p is prime.
   The first few terms are a(1) = -13, a(2) = -3, a(3) = 79, a(4) = 8101. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/pow2.h"
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

static seq_t *seq_cache = NULL;   /* dynamically grown array of terms */
static size_t cache_len = 0;       /* number of terms currently stored */
static size_t cache_cap = 0;       /* allocated capacity of the array */

/* Find the next prime after the given number.
   Uses a simple incremental test; p must be > 1. */
static idx_t next_prime(idx_t start)
{
    if (start <= 2) return 2;
    if (start == 3) return 3;
    if (start % 2 == 0) start++;   /* make odd */
    for (idx_t n = start; ; n += 2) {
        if (is_prime(n)) return n;
    }
}

/* Append a value to the cache, resizing if necessary. */
static void cache_append(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t newcap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *tmp = mem_resize(seq_t, seq_cache, newcap);
        if (!tmp) exit(EXIT_FAILURE);  /* allocation failed */
        seq_cache = tmp;
        cache_cap = newcap;
    }
    seq_cache[cache_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 0, but safe */
    /* Ensure cache contains at least n terms. */
    if (cache_len < n) {
        idx_t last_checked = (cache_len == 0) ? 1 : 0;  /* start searching from 2 */
        /* Find the largest p already processed for cache_len terms. */
        for (idx_t i = 1; i < cache_len; ++i) {
            /* _since we can't store p, we recompute it lazily_,
               but we don't need p here; we use incremental primes. */
        }
        /* Keep searching next primes and adding qualifying terms. */
        static idx_t last_prime = 1;   /* last prime examined when filling cache */
        while (cache_len < n) {
            last_prime = next_prime(last_prime + 1);
            /* Compute v = 2^p – 7p */
            seq_t v = pow2(last_prime) - ((seq_t)last_prime * 7);
            /* Test primality of |v|. Ignore |v| <= 1. */
            seq_t absv = (v < 0) ? -v : v;
            if (absv > 1 && is_prime_big(absv))
                cache_append(v);
        }
    }
    return seq_cache[n - 1];
}
