/*
 * A158711  Primes p such that p1=Floor[p/2]+p is prime and p2=Floor[p1/2]+p1
 *          is prime.
 * https://oeis.org/A158711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime p such that p1 = floor(p/2) + p is prime,
 *            and p2 = floor(p1/2) + p1 is prime. The code builds the terms on
 *            demand, storing them in a static dynamic array
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t *cache   = NULL;   /* a(1), a(2), … stored sequentially */
static size_t cache_len = 0;    /* number of cached terms */
static size_t cache_cap = 0;    /* capacity of the cache array */
static seq_t next_candidate = (seq_t)3;   /* next odd integer to test */

/* Append a newly found term to the cache, growing the array if needed. */
static void cache_append(seq_t value)
{
    if (cache_len == cache_cap) {
        /* Request a new capacity that can hold one more element. */
        grow_seq(&cache, &cache_cap, cache_len + 1);
    }
    cache[cache_len++] = value;
}

/* Ensure that at least n terms are cached. */
static void ensure_terms(idx_t n)
{
    /* Work in size_t (the cache length type) for the comparison. */
    while (cache_len < (size_t)n) {
        if (is_prime_big(next_candidate)) {
            seq_t p1 = (next_candidate / (seq_t)2) + next_candidate;
            seq_t p2 = (p1 / (seq_t)2) + p1;
            if (is_prime_big(p1) && is_prime_big(p2)) {
                cache_append(next_candidate);
            }
        }
        next_candidate += (seq_t)2;   /* skip even numbers */
    }
}

/* Main sequence access function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* Offset is 1; a(0) is unused. */
    ensure_terms(n);
    return cache[n - 1];
}
