/*
 * A167496  Primes p such that (p reversed)-10 is also a prime.
 * https://oeis.org/A167496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime p such that rev_decimal(p)-10 is prime
 *            (rev_decimal(p) is the decimal digit reversal of p)
 * Range:     u8 n <= 4, u16 n <= 140, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 936 (2026-11-03)
 * Helpers:   is_prime_u64, mem, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include <stdint.h>
#include "../utils/reverse_digits_idx.h"
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

static seq_t *cache = NULL;     /* Cached sequence values */
static size_t cache_len = 0;    /* Number of cached values */
static size_t cache_cap = 0;    /* Allocated capacity */
static idx_t  next_candidate = 2; /* Next integer to test */

/* Append a new term to the cache, resizing if needed. */
static void _append_term(seq_t term)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();          /* In production code handle error */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = term;
}

/* Return the n-th term of the sequence, 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* Defined safely although the offset is 1 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        idx_t cand = next_candidate++;
        if (!is_prime_big((seq_t)cand)) continue;  /* Candidate must itself be prime */

        idx_t rev_val = reverse_digits_idx(cand);
        if (rev_val < 10) continue;                /* rev(p)-10 would be negative */

        seq_t rev_minus = (seq_t)(rev_val - 10);
        if (!is_prime_big(rev_minus)) continue;    /* reversed−10 must also be prime */

        _append_term((seq_t)cand);
    }

    return cache[n - 1];
}
