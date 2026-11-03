/*
 * A033225  Primes of form x^2+37*y^2.
 * https://oeis.org/A033225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1619, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 8008 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A033225: primes p of the form x^2 + 37*y^2
 * Equivalently, primes ≡ {1,9,21,25,33,37,41,49,53,65,73,77,81,85,101,121,137,141,145} (mod 148) */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
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

/* Helper: allocate an array of seq_t and zero it. */
static seq_t *alloc_seq(size_t n)
{
    return mem_new(seq_t, n);
}

static const int residues[19] = {
    1, 9, 21, 25, 33, 37, 41, 49, 53,
    65, 73, 77, 81, 85, 101, 121, 137, 141, 145
};

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------
     * Static cache of found primes so far.  The cache grows automatically
     * whenever the requested index exceeds the current size.
     * ------------------------------------------------------------------ */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements stored */
    static size_t cache_cap  = 0;   /* allocated capacity */
    static uint64_t next_candidate = 37;   /* next integer to test for primality */

    /* Allocate or enlarge the cache if necessary */
    if (cache_size < (size_t)n) {
        /* Ensure we have enough space for the new elements */
        while (cache_size < (size_t)n) {
            /* Test the current candidate for primality */
            seq_t cand = (seq_t)next_candidate;
            if (is_prime_big(cand)) {
                /* Check if the prime has one of the allowed residues */
                int r = (int)(next_candidate % 148);
                for (int i = 0; i < 19; ++i) {
                    if (r == residues[i]) {
                        /* Add to cache, reallocating if needed */
                        if (cache_size == cache_cap) {
                            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                            if (!tmp) {    /* allocation failed – abort gracefully */
                                return 0;
                            }
                            cache = tmp;
                            cache_cap = new_cap;
                        }
                        cache[cache_size++] = cand;
                        break;
                    }
                }
            }
            /* Advance to the next integer */
            ++next_candidate;
        }
    }

    /* Return the cached value (a is 1‑based indexing) */
    return cache[n - 1];
}
