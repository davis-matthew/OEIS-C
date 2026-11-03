/*
 * A138700  Numbers k such that 2*k+1, 3*k+2, 4*k+3 and 5*k+4 are primes.
 * https://oeis.org/A138700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search method: iterate k ≡ 5 (mod 6) and test primality of
 * 2*k+1, 3*k+2, 4*k+3, and 5*k+4. */
#include <stdlib.h>
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

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Grow cache until it holds at least n terms. */
    while (cache_len < (size_t)n) {
        /* Current candidate k.  Start at 5 or continue from the
         * most recent term with the same congruence (k ≡ 5 mod 6). */
        seq_t k = (cache_len == 0) ? 5 : cache[cache_len - 1] + 6;

        /* Find next k satisfying the prime conditions. */
        for (;;) {
            if (is_prime_big(k * 2 + 1) &&
                is_prime_big(k * 3 + 2) &&
                is_prime_big(k * 4 + 3) &&
                is_prime_big(k * 5 + 4)) {

                /* Ensure space in cache. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
                break;      /* proceed to next term */
            }
            k += 6;          /* preserve k ≡ 5 (mod 6) */
        }
    }

    return cache[n - 1];   /* a(1) is the first element */
}
