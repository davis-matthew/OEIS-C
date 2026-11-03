/*
 * A174166  Composites n such that 2*n-1 is prime.
 * https://oeis.org/A174166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th composite integer m such that (2*m-1) is prime
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 995 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
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

/* Ensure the dynamic array has enough capacity */
static void ensure_seq_cap(size_t need, size_t *cap, seq_t **arr, size_t elem_size)
{
    if (need >= *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        if (need >= new_cap) new_cap = need + 1;
        *arr = mem_resize(seq_t, *arr, (new_cap * elem_size) / sizeof(seq_t));
        *cap = new_cap;
    }
}

/* a(n) implementation */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* stored terms, 1‑based index */
    static size_t cache_size = 0;        /* number of terms currently cached   */
    static size_t cache_cap  = 0;        /* allocated capacity for cache        */
    static idx_t next_candidate = 2;     /* next integer to test for eligibility */

    if (n == 0) return (seq_t)0; /* undefined; safe fallback */

    /* Grow cache until it contains the requested term */
    while (cache_size < n) {
        /* Find next eligible composite */
        while (true) {
            /* Skip primes and 1 */
            if (next_candidate > 1 && !is_prime_big((seq_t)next_candidate)) {
                seq_t p = ((seq_t)2 * (seq_t)next_candidate) - (seq_t)1;
                if (is_prime_big(p)) {
                    /* Eligible – store it */
                    ensure_seq_cap(cache_size + 1, &cache_cap, &cache, sizeof(seq_t));
                    cache[cache_size++] = (seq_t)next_candidate;
                    ++next_candidate; /* advance after using it */
                    break;
                }
            }
            ++next_candidate;
        }
    }

    return cache[n - 1];
}
