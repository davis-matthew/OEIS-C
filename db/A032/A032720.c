/*
 * A032720  Integers that when prefixed by '3' and followed by '7' yield a
 *          prime.
 * https://oeis.org/A032720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032720: a(n) = the nth integer k (n≥1) such that the decimal
   number formed by prefixing k with '3' and suffixing it with '7'
   (i.e., 3·10^(digits(k)+1)+k·10+7) is prime.  The function
   computes this value by a linear search with memoisation. */

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

static inline int num_digits(idx_t x)
{
    if (x == 0) return 1;
    int d = 0;
    while (x > 0) {
        x /= 10;
        ++d;
    }
    return d;
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache  = NULL;
    static size_t cap    = 0;
    static size_t found  = 0;
    static idx_t  cur_k  = 0; /* next candidate integer */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap   = 16;
        cache = mem_new(seq_t, cap);
        if (!cache) abort();
    }

    size_t target = (size_t)n;
    while (found < target) {
        /* number of decimal digits of cur_k */
        int d = num_digits(cur_k);

        /* compute 10^(d+1) as seq_t */
        seq_t pow10_val = 1;
        for (int i = 0; i < d + 1; ++i)
            pow10_val *= (seq_t)10;

        /* build the number 3k7 */
        seq_t num = (seq_t)3 * pow10_val + (seq_t)cur_k * (seq_t)10 + (seq_t)7;

        /* test primality */
        if (is_prime_big(num)) {
            /* ensure cache capacity */
            if (found >= cap) {
                size_t new_cap = cap * 2 + 1;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cap   = new_cap;
            }
            cache[found] = (seq_t)cur_k;
            ++found;
        }
        ++cur_k;
    }

    return cache[found - 1];
}
