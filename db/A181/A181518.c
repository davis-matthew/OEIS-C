/*
 * A181518  a(n) is the number for which 2^A181516(n)||(2*a(n))!
 * https://oeis.org/A181518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A181518
   Numbers m such that Sum_{k>=0} floor(m/2^k) is prime.
   Equivalent to: 2*m - popcount(m) is prime.
   a(1) = 2, a(2) = 4, a(3) = 7, a(4) = 11, ...
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>
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
    /* n >= 1; if n==0 return 0 as a sentinel */
    if (n == 0) return (seq_t)0;

    /* cache of previously found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0; /* number of cached terms */
    static size_t cache_cap   = 0; /* allocated capacity of cache */
    static uint64_t next_candidate = 1; /* next m to test */

    while (cache_size < (size_t)n) {
        uint64_t cand = next_candidate++;
        unsigned pc = __builtin_popcountll(cand);

        /* compute 2*cand - pc as seq_t */
        seq_t val = ((seq_t)cand * (seq_t)2) - (seq_t)pc;

        if (is_prime_big(val)) {
            /* enlarge cache if necessary */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) {
                    /* allocation failed – fallback to 0 */
                    mem_free(cache);
                    cache = NULL;
                    cache_size = 0;
                    cache_cap   = 0;
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
