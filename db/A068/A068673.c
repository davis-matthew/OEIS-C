/*
 * A068673  Numbers which yield primes when a 1 is prefixed or appended.
 * https://oeis.org/A068673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A068673: a(n) = the n-th positive integer n such that 
 *    10^digits(n)+n is prime and n*10+1 is prime
 * This implementation iteratively searches numbers and memoizes the
 * results so subsequent calls are fast.
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

seq_t a(idx_t n) {
    /* static state for memoization */
    static seq_t *cache = NULL;      /* array of found terms */
    static size_t cache_len = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t last_checked = 0;   /* last integer checked */

    /* variables for power of 10 management */
    static seq_t pow10 = 0;         /* 10^digits(current) */
    static idx_t next_limit = 0;    /* next integer where digit count increases */
    static int digit_len = 0;       /* current number of digits */

    /* one-time initialization */
    if (cache_len == 0) {
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
        pow10 = (seq_t)10;       /* 10^1 for numbers 1..9 */
        next_limit = 10;         /* next multiplication threshold */
        digit_len = 1;
        last_checked = 0;
    }

    /* return immediately if we already have the term */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* allocate more space when needed */
    if (cache_len >= cache_cap) {
        cache_cap *= 2;
        cache = mem_resize(seq_t, cache, cache_cap);
    }

    /* search for new terms until we have at least n of them */
    for (;;) {
        idx_t val = ++last_checked;     /* candidate integer */

        /* update digit length and corresponding power of 10 */
        if (val == next_limit) {
            digit_len++;
            pow10 *= (seq_t)10;
            next_limit *= 10;
        }

        seq_t seq_val = (seq_t)val;
        seq_t prefix = seq_val + pow10;         /* 10^digits + n */
        seq_t suffix = seq_val * (seq_t)10 + 1; /* n*10 + 1 */

        if (is_prime_big(prefix) && is_prime_big(suffix)) {
            /* store the found term */
            cache[cache_len++] = seq_val;
            if (cache_len == n) {   /* we have enough terms */
                return seq_val;
            }
        }
    }
}
