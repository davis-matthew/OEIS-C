/*
 * A075707  Safe primes (A005385) (p and (p-1)/2 are primes) such that 12*p+1
 *          is also prime.
 * https://oeis.org/A075707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 79 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A075707: safe primes p (p and (p-1)/2 prime) such that 12*p+1 is prime. */
/* The function returns the n-th such prime for any n ≥ 1. */

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

/* ---------- cache of discovered sequence elements ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static void ensure_cache(size_t newlen) {
    if (newlen <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 64;
    while (newcap < newlen) newcap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, newcap);
    if (!tmp) abort();
    cache = tmp;
    cache_cap = newcap;
}
static void cache_append(seq_t val) {
    ensure_cache(cache_len + 1);
    cache[cache_len++] = val;
}

/* ---------- generator of sequence terms ---------- */
static seq_t next_candidate = 5;   /* first odd ≥5 */
static bool initialized = false;

static void generate_terms(idx_t n) {
    if (!initialized) { next_candidate = 5; initialized = true; }
    while (cache_len < (size_t)n) {
        /* test candidate for primality */
        if (is_prime_big(next_candidate)) {
            /* (p-1)/2 must be prime */
            seq_t half = (next_candidate - 1) / 2;
            if (is_prime_big(half)) {
                /* 12*p+1 must be prime */
                seq_t mp = 12 * next_candidate + 1;
                if (is_prime_big(mp))
                    cache_append(next_candidate);
            }
        }
        next_candidate += 2;   /* skip even numbers */
    }
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n) {
    if (n < 1) return 0;
    generate_terms(n);
    return cache[n - 1];
}
