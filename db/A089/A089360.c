/*
 * A089360  Prime worms.
 * https://oeis.org/A089360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the n‑th prime whose decimal digits satisfy the
 *            prime‑worm conditions: at least 3 digits, identical first and
 *            last digits, no two adjacent digits equal, and all adjacent
 *            absolute differences are the same. This implementation searches
 *            primes to produce the term
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   get_digits, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_digits.h"
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

/* Test whether a seq_t value has any pair of consecutive equal decimal digits. */
static inline bool no_consecutive_equal_digits(seq_t val) {
    if (val == (seq_t)0) return true;          /* 0 contains only one digit */
    int prev = -1;                             /* previous digit, -1 means none yet */
    seq_t tmp = val;
    while (tmp > (seq_t)0) {
        int d = (int)(tmp % (seq_t)10);        /* current least‑significant digit */
        if (prev == d) return false;           /* found equal neighbours */
        prev = d;
        tmp /= (seq_t)10;                      /* shift right one decimal digit */
    }
    return true;                               /* all adjacent digits differ */
}

/* Helper that tests a prime for the prime‑worm conditions. */
static bool is_worm_prime(seq_t p)
{
    /* Must be at least 3 decimal digits. */
    if (p < (seq_t)101)
        return false;

    /* Quick reject if adjacent digits are equal. */
    if (!no_consecutive_equal_digits(p))
        return false;

    /* Gather decimal digits in most‑significant order. */
    uint8_t buf[128];
    size_t len = 0;
    get_digits(p, buf, &len);
    if (len < 3)
        return false;

    /* First and last digits must match. */
    if (buf[0] != buf[len - 1])
        return false;

    /* Constant absolute difference between successive digits. */
    int diff = (buf[0] > buf[1]) ? buf[0] - buf[1] : buf[1] - buf[0];
    if (diff == 0)           /* adjacent digits already checked, but keep safety. */
        return false;
    for (size_t i = 1; i + 1 < len; ++i) {
        int cur = (buf[i] > buf[i + 1]) ? buf[i] - buf[i + 1] : buf[i + 1] - buf[i];
        if (cur != diff)
            return false;
    }

    /* Finally verify primality. */
    return is_prime_big(p);
}

seq_t a(idx_t n)
{
    /* Cache of previously found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Current candidate to test; start at the smallest 3‑digit prime. */
    static seq_t next_cand = (seq_t)101;

    while (cache_len < n) {
        seq_t cand = next_cand;
        /* Advance to the next odd number. All primes >2 are odd. */
        next_cand += (seq_t)2;

        if (is_worm_prime(cand)) {
            /* Grow cache if necessary. */
            if (cache_len >= cache_cap) {
                cache_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                cache = tmp;
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[(size_t)(n - 1)];
}
