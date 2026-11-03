/*
 * A032717  Numbers k such that k prefixed by '9' and followed by '3' is
 *          prime.
 * https://oeis.org/A032717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) returns the n‑th integer k such that the number 9 k 3 * (9
 *            concatenated with k followed by 3) is prime. No simpler * closed
 *            form is known. The function memoizes found terms so that * calls
 *            are O(1) after the term has been discovered. All arithmetic * is
 *            performed in the type seq_t, which may...
 * Range:     u8 none, u16 n <= 26, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9530 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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
    static idx_t last_checked = 0;   /* largest k examined so far      */
    static seq_t *cache = NULL;      /* array of discovered k values   */
    static size_t cache_len = 0;     /* number of terms stored         */
    static size_t cache_cap = 0;     /* current capacity of cache      */
    static seq_t pow_cache = 0;      /* 10^(len(k)+1) for current k    */
    static int  pow_digits = -1;     /* len(k)+1 that pow_cache holds  */

    if (n == 0) return 0;           /* offset is 1, undefined below */

    if (n <= cache_len)             /* value already memoized */
        return cache[n - 1];

    if (cache == NULL) {            /* allocate initial cache      */
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) abort();
    }

    for (idx_t k = last_checked + 1; ; ++k) {
        int len = num_digits_idx(k);       /* number of decimal digits of k */
        int target = len + 1;         /* exponent for 10^(len+1)     */
        if (target != pow_digits) {
            pow_cache = pow10((idx_t)target);
            pow_digits = target;
        }
        seq_t num = pow_cache * 9 + (seq_t)k * 10 + 3;  /* 9k3 */
        if (is_prime_big(num)) {              /* prime test */
            if (cache_len >= cache_cap) {     /* grow cache if needed */
                size_t new_cap = cache_cap * 2;
                if (new_cap < cache_len + 1)
                    new_cap = cache_len + 1;
                seq_t *newptr = mem_resize(seq_t, cache, new_cap);
                if (!newptr) abort();
                cache    = newptr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
            if (cache_len >= n) {
                last_checked = k;
                return cache[n - 1];
            }
        }
        last_checked = k;   /* advance to next candidate             */
    }
}
