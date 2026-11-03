/*
 * A142094  Primes congruent to 27 mod 35.
 * https://oeis.org/A142094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 277, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 973 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth prime p with p ≡ 27 (mod 35) */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
    /* static state for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static idx_t tested = 0;          /* number of candidates inspected */

    /* first‑time allocation */
    if (cache == NULL) {
        cache_cap = 1024;                     /* initial capacity */
        cache = mem_new(seq_t, cache_cap);
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
    }

    /* generate primes until we have at least n of them */
    while (cache_len < n) {
        /* candidate of the form 27 + 35*k, where k = tested */
        seq_t candidate = (seq_t)(27 + (seq_t)35 * tested);

        if (is_prime_big(candidate)) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap << 1;
                seq_t *tmp = mem_new(seq_t, new_cap);
                if (tmp == NULL) return (seq_t)0; /* allocation failure */
                mem_copy(tmp, cache, sizeof(seq_t) * cache_len);
                mem_free(cache);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }
        tested++; /* advance to the next candidate */
    }

    return cache[n - 1];
}
