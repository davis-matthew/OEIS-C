/*
 * A172410  Numbers k such that 2k + 9 and 2k + 27 are prime.
 * https://oeis.org/A172410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = the n-th k such that 2k+9 and 2k+27 are prime
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 978 (2026-11-03)
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

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n)
{
    /* static cache of found k's */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* allocated array capacity */
    static idx_t  tested_k  = 0;    /* largest k that has been tested */

    /* generate until we have at least n terms */
    while (cache_size < n) {
        /* move to next k */
        tested_k += 1;
        seq_t k  = (seq_t)tested_k;                   /* k itself */
        seq_t p1 = k * ((seq_t)2) + ((seq_t)9);       /* 2k+9 */
        seq_t p2 = k * ((seq_t)2) + ((seq_t)27);      /* 2k+27 */

        if (is_prime_big(p1) && is_prime_big(p2)) {
            /* grow cache array if needed */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2U : 4U;
                grow_seq_array(&cache, &cache_cap, new_cap);
            }
            cache[cache_size++] = k;   /* append new candidate */
        }
    }
    return cache[n - 1];  /* a(1) is the first stored term */
}
