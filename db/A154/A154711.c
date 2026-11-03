/*
 * A154711  Squares such that square-+5 are primes.
 * https://oeis.org/A154711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154711: squares n^2 such that n^2-5 and n^2+5 are prime. */
/* Calculation uses a simple incremental search with memoisation. */

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

seq_t a(idx_t n)
{
    /* Static state for memoisation across calls. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t checked = 0;          /* last integer checked for the property */

    if (n == 0)        /* the sequence is 1‑based; 0 is undefined */
        return 0;

    /* Extend cache until it holds at least n terms. */
    while (cache_size < n) {
        ++checked;                     /* next candidate integer */

        /* Compute checked² as a seq_t value. */
        seq_t sq = (seq_t)checked;
        sq *= sq;

        /* Skip trivial cases where sq-5 would not be prime. */
        if (sq <= 5) continue;

        /* Test primality of sq-5 and sq+5. */
        if (is_prime_big(sq - 5) && is_prime_big(sq + 5)) {
            /* Grow the cache if necessary. */
            if (cache_size == cache_cap) {
                size_t newcap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, newcap);
                if (!tmp) abort();   /* allocation failure */
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_size++] = sq;
        }
    }

    return cache[n - 1];
}
