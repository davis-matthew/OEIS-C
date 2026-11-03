/*
 * A180933  Initial segments of A010051, interpreted as binary numbers, that
 *          are prime.
 * https://oeis.org/A180933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A180933:
 *   a(n) = the n‑th integer obtained by interpreting the first m
 *   terms of A010051 (the prime characteristic function) as a binary
 *   number, for increasing m, and keeping those prefixes that are
 *   themselves prime.  For example, the first few primes are
 *   2,3,5,7,… giving the binary string 1101…, whose prefixes 11 (3),
 *   1101 (13), 110101 (53) … are prime, hence a(1)=3, a(2)=13,
 *   a(3)=53, etc. */

/* Note: The implementation uses the helpers
 *   is_prime(idx_t)   – tests primality of an ordinary integer,
 *   is_prime_big(seq_t) – tests primality of an arbitrary‑precision
 *   integer.  All other utilities are provided by the framework. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#include <stdint.h>
#include "../utils/is_prime.h"
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
/* Cached results.  The cache grows geometrically.                       */
/* --------------------------------------------------------------------- */
static seq_t *cache      = NULL;   /* array of computed primes      */
static size_t cache_len  = 0;      /* number of primes stored       */
static size_t cache_cap  = 0;      /* current capacity of the cache */

static seq_t current_prefix = 0;   /* current binary prefix value   */
static idx_t  next_k        = 2;   /* next integer to test for bit  */

/* --------------------------------------------------------------------- */
/* Grow cache to hold at least 'need' entries.  This is safe for both */
/* C and C++ because we cast the void* returned by realloc.             */
/* --------------------------------------------------------------------- */
static void
grow_cache_if_needed(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *p = mem_resize(seq_t, cache, new_cap);
    if (!p) abort();   /* out of memory */
    cache = p;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Generate terms until we have at least 'need' of them.                 */
/* --------------------------------------------------------------------- */
static void
generate_terms(idx_t need)
{
    while (cache_len < (size_t)need) {
        bool bit = is_prime(next_k);      /* 1 if next_k is prime  */
        current_prefix = (current_prefix << 1) | (seq_t)bit;
        if (is_prime_big(current_prefix)) {
            grow_cache_if_needed(cache_len + 1);
            cache[cache_len++] = current_prefix;
        }
        ++next_k;
    }
}

/* --------------------------------------------------------------------- */
/* Return the n-th term of the sequence (1‑based).                        */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range – not used */
    generate_terms(n);
    return cache[n - 1];
}
