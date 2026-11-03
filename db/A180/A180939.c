/*
 * A180939  Numbers n such that n^2 - 2999n + 2248541 is prime.
 * https://oeis.org/A180939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth integer n such that n^2 - 2999*n + 2248541 is
 *            prime
 * Range:     u8 n <= 2, u16 none, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-03)
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

/* Return the nth term of A180939.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached sequence values */
    static size_t sz   = 0;       /* number of cached terms */
    static size_t cap  = 0;       /* allocated capacity of cache */
    static idx_t  next_candidate = 1;   /* next integer to test */

    /* Ensure the cache contains at least n terms. */
    while (sz < n) {
        idx_t cand = next_candidate++;   /* candidate integer */

        /* Evaluate the quadratic polynomial in seq_t arithmetic. */
        seq_t val = (seq_t)cand * (seq_t)cand
                  - ((seq_t)2999 * (seq_t)cand)
                  + (seq_t)2248541;

        /* Test for primality. */
        if (is_prime_big(val)) {
            /* Grow the cache if full. */
            if (sz == cap) {
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cap   = new_cap;
            }
            cache[sz++] = (seq_t)cand;   /* store the integer itself */
        }
    }

    return cache[(size_t)(n - 1)];
}
