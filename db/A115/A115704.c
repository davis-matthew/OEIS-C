/*
 * A115704  Triangular numbers whose digit reversal is prime; trailing zeros
 *          are permitted.
 * https://oeis.org/A115704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 52 terms through n = 975 (2026-11-03)
 * Helpers:   is_prime_u64, mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A115704: Triangular numbers whose digit reversal is prime; trailing zeros allowed.
   Implementation: generate triangular numbers sequentially and keep a cache
   of those whose reverse is prime.  Uses the supplied utilities:
   - rev_decimal(seq_t)   to reverse digits
   - is_prime_big(seq_t)  to test primality of the reversed number. */

#include <stdlib.h>
#include <stdbool.h>

#include <stdint.h>
#include "../utils/reverse_digits.h"
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

/* Helper to append a term to the dynamic cache */
static void append_term(seq_t val, seq_t **cache, size_t *len, size_t *cap)
{
    if (*len == *cap) {
        size_t new_cap = (*cap > 0) ? (*cap << 1) : 16;
        *cache = mem_resize(seq_t, *cache, new_cap);
        *cap = new_cap;
    }
    (*cache)[*len] = val;
    (*len)++;
}

/* Return the nth term of A115704 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Undefined for 0, return a safe value */

    /* Static state to cache results between calls */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_tri = 0;  /* last triangular index processed */

    /* Ensure we have at least n qualifying terms cached */
    while (cache_len < (size_t)n) {
        last_tri++;  /* next triangular index */
        seq_t t = (seq_t)last_tri * ((seq_t)last_tri + 1) / 2;  /* t_n */
        seq_t rev = reverse_digits(t);
        if (is_prime_big(rev)) {
            append_term(t, &cache, &cache_len, &cache_cap);
        }
    }

    return cache[(size_t)n - 1];
}
