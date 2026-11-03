/*
 * A161616  Primes p such that 2*p+111546435 is also prime.
 * https://oeis.org/A161616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   primes p such that 2*p + 111546435 is also prime
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 69 terms through n = 880 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Memoised cache for the sequence terms. */
static seq_t *cache      = NULL;   /* array of found primes p */
static size_t cache_len  = 0;      /* number of stored terms */
static size_t cache_cap  = 0;      /* capacity of the array */

/* Next odd candidate to test; 2 is handled separately. */
static idx_t next_candidate = 2;

/* Return the n-th term of A161616 (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined behaviour for n<1, return 0 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* Test candidate for primality. */
        if (next_candidate == 2) {
            /* 2 is the only even prime. */
            if (is_prime_seq((seq_t)2)) {
                seq_t q = ((seq_t)2) * 2 + (seq_t)111546435;
                if (is_prime_big(q)) {
                    seq_t *tmp = seq_realloc(cache, cache_cap, cache_len + 1);
                    if (!tmp) return 0;   /* allocation failed */
                    cache = tmp;
                    cache_cap = cache_len + 1;
                    cache[cache_len] = (seq_t)2;
                    cache_len++;
                }
            }
            next_candidate = 3;          /* next candidate will be odd */
        } else {
            /* Only test odd numbers. */
            seq_t cand = (seq_t)next_candidate;
            if (is_prime_seq(cand)) {
                seq_t q = cand * 2 + (seq_t)111546435;
                if (is_prime_big(q)) {
                    seq_t *tmp = seq_realloc(cache, cache_cap, cache_len + 1);
                    if (!tmp) return 0;   /* allocation failed */
                    cache = tmp;
                    cache_cap = cache_len + 1;
                    cache[cache_len] = cand;
                    cache_len++;
                }
            }
            /* Advance to next odd integer. */
            next_candidate += 2;
        }
    }

    /* Return the cached value. */
    return cache[n - 1];
}
