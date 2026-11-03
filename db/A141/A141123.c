/*
 * A141123  Primes of the form -x^2+2*x*y+2*y^2 (as well as of the form
 *          3*x^2+6*x*y+2*y^2).
 * https://oeis.org/A141123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1649, u32 n <= 6514, u64 n <= 6514,
 *            u128 n <= 6514, big any n
 * Validated: standard, pass, 68 terms through n = 5969 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) is 2, then 3, then the primes that are congruent to 11 (mod 12). */
/* The implementation builds the sequence on demand, caching results. */

#include <stdint.h>
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static seq_t *cache = NULL;   /* cached sequence terms */
static size_t cache_len = 0;   /* number of cached terms */
static size_t cache_cap = 0;   /* capacity of the cache array */
static idx_t next_candidate = 2;   /* next integer to test */

/* Return the nth term (1‑based) of A141123. */
seq_t a(idx_t n)
{
    /* Fast path: return cached value if already computed. */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Extend the sequence until we have at least n terms. */
    while (cache_len < (size_t)n)
    {
        /* Find the next prime that satisfies the required form. */
        while (true)
        {
            if (is_prime_big((seq_t)next_candidate) &&
                (next_candidate == 2 || next_candidate == 3 ||
                 (next_candidate % 12) == 11))
            {
                /* Store it in the cache. */
                append_candidate(&cache, &cache_len, &cache_cap,
                                 (seq_t)next_candidate);
                /* Move past this prime for the next search. */
                ++next_candidate;
                break;
            }
            ++next_candidate;
        }
    }

    return cache[n - 1];
}
