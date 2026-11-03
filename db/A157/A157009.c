/*
 * A157009  Primes p such that 3 = abs(largest digit of p - sum of all the
 *          other digits of p).
 * https://oeis.org/A157009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime p such that 3 = abs(max digit of p - sum of
 *            all the other digits of p)
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   digit_counts, is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include "../utils/digit_counts.h"
#include "../utils/is_prime_seq.h"
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

/* simple absolute value for int */
static inline int abs_int(int x) { return x < 0 ? -x : x; }

seq_t a(idx_t n) {
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0, cache_len = 0;
    if (n == 0) return 0;  /* undefined, but avoid UB */

    while (cache_len < n) {
        seq_t candidate = (cache_len == 0) ? 2 : cache[cache_len - 1] + 1;
        while (1) {
            bool prime;
            if (sizeof(seq_t) <= 8) {
                prime = is_prime_seq((uint64_t)candidate);
            } else {
                prime = is_prime_big(candidate);
            }
            if (!prime) {
                candidate++;
                continue;
            }

            unsigned char cnt[10] = {0};
            digit_counts(candidate, cnt);

            int sum = 0, maxd = 0;
            for (int d = 0; d <= 9; d++) {
                if (cnt[d]) {
                    sum += d * cnt[d];
                    if (d > maxd) maxd = d;
                }
            }
            int diff = abs_int(2 * maxd - sum);
            if (diff == 3) {
                /* append to cache */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) return 0;  /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = candidate;
                break;
            }
            candidate++;
        }
    }
    return cache[n - 1];
}
