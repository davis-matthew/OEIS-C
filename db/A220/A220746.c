/*
 * A220746  Numbers n such that n and n + 10 are prime and there is a power of
 *          two in the interval (n, n+10).
 * https://oeis.org/A220746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer n such that n and n+10 are prime and
 *            there exists a power of two 2^k with n < 2^k < n+10
 * Range:     u8 none, u16 n <= 7, u32 none, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   is_prime_u64, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"
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
    /* static cache for generated terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* state for generating new terms */
    static idx_t cur_k = 1;          /* current exponent of two   */
    static seq_t cur_power;          /* 2^cur_k */
    static seq_t last_n = 0;         /* last inserted term */

    /* initialise static data on first call */
    if (cache == NULL) {
        cache_cap   = 128;
        cache       = mem_new(seq_t, cache_cap);
        cache_len   = 0;
        cur_k       = 1;
        cur_power   = pow2(cur_k);   /* 2^1 = 2 */
        last_n      = 0;
    }

    /* generate terms until we have at least n entries */
    while (cache_len < n) {
        /* For the current power of two, test all possible offsets d
           such that the power lies between (2^k - d, 2^k + (10-d)). */
        for (int d = 9; d >= 1; --d) {
            seq_t cand = cur_power - d;
            if (cand < (seq_t)2)            /* candidate must be >= 2 */
                continue;
            seq_t cand10 = cur_power + ((seq_t)10 - (seq_t)d);

            if (is_prime_big(cand) && is_prime_big(cand10)) {
                if (cand > last_n) {       /* keep ascending order */
                    /* grow cache if necessary */
                    if (cache_len == cache_cap) {
                        cache_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
                        cache = mem_resize(seq_t, cache, cache_cap);
                    }
                    cache[cache_len++] = cand;
                    last_n = cand;
                    if (cache_len >= n)
                        break;
                }
            }
        }
        if (cache_len >= n)
            break;

        /* move to the next power of two */
        ++cur_k;
        cur_power = pow2(cur_k);
    }

    return cache[n - 1];
}
