/*
 * A228359  Numbers m whose 10's complement is not prime, i.e., 10^k-m, where
 *          k is the number of digits of m, is not prime.
 * https://oeis.org/A228359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, less
 * Formula:   a(n) is the nth positive integer m such that 10^digits(m) – m is
 *            not prime
 * Range:     u8 n <= 120, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_count, grow_seq, isPrimeGeneric, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_count.h"   /* digit_count */
#include "../utils/pow10.h"         /* pow10 */
#include "../utils/isPrimeGeneric.h"/* isPrimeGeneric */
#include "../utils/grow_seq.h"      /* grow_seq */

static seq_t *cache        = NULL;  /* stored sequence values */
static size_t cache_cap    = 0;     /* allocated capacity of cache */
static idx_t  cache_cnt    = 0;     /* number of values stored in cache */
static idx_t  next_m       = 1;     /* next candidate integer to test */

/* Ensure cache has at least n entries. */
static void ensure_cache(idx_t n)
{
    while (cache_cnt < n) {
        seq_t cand = (seq_t)next_m;
        /* number of decimal digits of cand (stored in a 64‑bit type to avoid overflow) */
        idx_t d = (idx_t)digit_count((uint64_t)next_m);
        seq_t pow = pow10(d);              /* 10^d as seq_t */
        seq_t comp = pow - cand;           /* 10^d – m */
        bool prime = isPrimeGeneric(comp);
        if (!prime) {                      /* composite or 1 */
            if (cache_cnt == cache_cap) {
                size_t new_cap = cache_cap? cache_cap * 2 : 1;
                grow_seq(&cache, &cache_cap, new_cap);
            }
            cache[cache_cnt++] = cand;
        }
        ++next_m;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n<1; return 0 as safe default */
    ensure_cache(n);
    return cache[n-1];
}
