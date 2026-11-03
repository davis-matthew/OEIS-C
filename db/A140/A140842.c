/*
 * A140842  Primes of the form 210k + 17.
 * https://oeis.org/A140842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime of the form 210k + 17 (k ≥ 0)
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
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

/* A small dynamic cache of previously found primes of the required form */
static seq_t *prime_cache    = NULL;  /* array of primes */
static size_t prime_cache_size = 0;   /* number of stored primes */
static size_t prime_cache_cap  = 0;   /* current allocated capacity */
static idx_t   next_k           = 0;  /* next value of k to test */

/* Append a new prime to the cache, expanding storage if necessary */
static void cache_push(seq_t value)
{
    if (prime_cache_size == prime_cache_cap) {
        size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 8;
        seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
        if (!tmp) {
            /* Allocation failed – terminate as the sequence cannot be continued */
            exit(EXIT_FAILURE);
        }
        prime_cache   = tmp;
        prime_cache_cap = new_cap;
    }
    prime_cache[prime_cache_size++] = value;
}

/* Return the n-th prime of the form 210k + 17, 1‑based indexing */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is undefined for n=0; return zero to signal invalid index */
        return 0;
    }

    /* If the requested prime is already cached, return it immediately */
    if ((size_t)(n - 1) < prime_cache_size) {
        return prime_cache[n - 1];
    }

    /* Generate further candidates until the cache holds at least n primes */
    while (prime_cache_size < n) {
        seq_t candidate = 210 * (seq_t)next_k + 17;
        if (is_prime_big(candidate)) {
            cache_push(candidate);
        }
        ++next_k;
    }

    return prime_cache[n - 1];
}
