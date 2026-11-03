/*
 * A153421  Primes of the form 2*n^2+11.
 * https://oeis.org/A153421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the nth prime of the form 2*k^2 + 11 (OEIS entry
 *            A153421). The sequence is computed by testing successive values
 *            of k (starting at 0) and keeping the primes found
 * Range:     i8 n <= 8, i16 n <= 14, i32 n <= 14, i64 n <= 14, i128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
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

static seq_t *cache = NULL;          /* array of found primes */
static size_t cache_len = 0;         /* number of elements stored      */
static size_t cache_cap = 0;         /* current array capacity         */
static uint64_t next_k = 0;          /* next k to test  (k starts 0)   */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for 0, but safe      */

    /* expand cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        seq_t k   = (seq_t)next_k;
        seq_t val = (seq_t)2 * k * k + (seq_t)11;

        if (is_prime_big(val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) {
                    /* allocation failed – return 0 (sequence undefined) */
                    return 0;
                }
                cache    = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        ++next_k;
    }

    return cache[n - 1];
}
