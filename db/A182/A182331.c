/*
 * A182331  Primes of the form 6^k + 1.
 * https://oeis.org/A182331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   ipow, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form 6^k + 1.
   k runs over 0,1,2,4,8,16,… (powers of two, with 0 allowed).
   The function generates primes in that order, caching results
   for subsequent calls. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"
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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    static idx_t exp = 0;              /* current exponent k */
    static const idx_t MAX_EXP = ((idx_t)1 << 28);   /* maximum k to test */

    /* lazy allocation of cache */
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return (seq_t)0;   /* allocation failure */
    }

    /* Generate primes until we have at least n terms or we run out of k. */
    while (cache_len < (size_t)n) {
        if (exp > MAX_EXP) break;     /* no more candidates to test */

        /* candidate = 6^exp + 1 */
        seq_t cand = ipow((seq_t)6, (seq_t)exp);
        cand += (seq_t)1;

        if (is_prime_big(cand)) {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 1;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return (seq_t)0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }

        /* Advance exponent to the next power of two. */
        if (exp == 0)
            exp = 1;
        else
            exp <<= 1;
    }

    if (cache_len < (size_t)n) return (seq_t)0;
    return cache[n - 1];
}
