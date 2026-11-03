/*
 * A057659  Prime numbers whose square is composed of just two different
 *          decimal digits.
 * https://oeis.org/A057659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) is the n-th prime whose square consists of exactly two
 *            distinct decimal digits
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static seq_t *prime_cache = NULL;   /* stored qualifying primes       */
static size_t cache_len   = 0;       /* number of cached primes        */
static size_t cache_cap   = 0;       /* allocated capacity of cache    */
static idx_t  last_checked = 1;      /* last integer examined for prime */

seq_t a(idx_t n)
{
    /* n must be positive; for n==0, return 0 (undefined) */
    if (n == 0) return 0;

    /* Grow the cache until it contains at least n items */
    while (cache_len < n) {
        last_checked++;

        /* Skip even numbers greater than 2 */
        if (last_checked > 2 && (last_checked & 1) == 0)
            continue;

        /* Prime test */
        if (!is_prime_big((seq_t)last_checked))
            continue;

        /* Square of the prime */
        seq_t sq = (seq_t)last_checked * (seq_t)last_checked;

        /* Count distinct digits of the square */
        unsigned char seen[10] = {0};
        size_t distinct = 0;
        seq_t tmp = sq;
        if (tmp == 0) {
            seen[0] = 1;
            distinct = 1;
        } else {
            while (tmp > 0) {
                int d = (int)(tmp % 10);
                if (!seen[d]) {
                    seen[d] = 1;
                    ++distinct;
                    if (distinct > 2) break;
                }
                tmp /= 10;
            }
        }

        /* Accept if exactly two distinct digits */
        if (distinct == 2) {
            /* Ensure enough space in the cache */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp_buf = mem_resize(seq_t, prime_cache, new_cap);
                if (!tmp_buf) abort();   /* allocation failure */
                prime_cache = tmp_buf;
                cache_cap = new_cap;
            }
            prime_cache[cache_len++] = (seq_t)last_checked;
        }
    }

    /* Return the cached prime (1‑based index) */
    return prime_cache[n - 1];
}
