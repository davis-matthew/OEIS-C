/*
 * A152426  Primes that have both prime digits (2,3,5,7) and nonprime digits
 *          (0,1,4,6,8,9).
 * https://oeis.org/A152426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   get_digits, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes having both prime digits (2,3,5,7) and nonprime digits (0,1,4,6,8,9) */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
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

/* Cache for the sequence values.  The cache grows as needed. */
static seq_t *cache = NULL;   /* array of found terms          */
static size_t cache_len = 0;  /* number of terms stored        */
static size_t cache_cap = 0;  /* current capacity of the array */

/* Current candidate being tested.  It is preserved across calls. */
static seq_t candidate = 0;
static bool init_candidate = true;

/* Ensure that the cache has room for at least `needed` terms. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    if (new_cap < needed)
        new_cap = needed;
    cache = mem_resize(seq_t, cache, new_cap);
    cache_cap = new_cap;
}

/* Test whether a prime has at least one prime digit (2,3,5,7) and one
   non‑prime digit (0,1,4,6,8,9). */
static bool has_required_digits(seq_t p)
{
    uint8_t digits[128];
    size_t dlen;
    get_digits(p, digits, &dlen);

    bool has_prime  = false;
    bool has_nonpr = false;
    for (size_t i = 0; i < dlen; ++i) {
        uint8_t d = digits[i];
        if (d == 2 || d == 3 || d == 5 || d == 7)
            has_prime = true;
        else
            has_nonpr = true;
    }
    return has_prime && has_nonpr;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for index 0, not used */

    /* Initialize the candidate counter only once. */
    if (init_candidate) {
        candidate = (seq_t)2;
        init_candidate = false;
    }

    /* Grow the cache until we have at least `n` terms. */
    while (cache_len < (size_t)n) {
        seq_t cand = candidate;

        /* Get the last digit to cheaply skip many composites. */
        uint8_t digits[128];
        size_t dlen;
        get_digits(cand, digits, &dlen);
        uint8_t last_digit = (dlen > 0) ? digits[0] : 0;  /* digits[0] = least‑significant */

        /* Skip obvious composites: even last digit or ending in 5,
           except for the actual primes 2 and 5 themselves. */
        if (cand > (seq_t)5 &&
            (last_digit == 5 || ((last_digit & 1) == 0))) {
            /* advance to next integer */
            ++candidate;
            continue;
        }

        /* Test primality and digit condition. */
        if (is_prime_big(cand) && has_required_digits(cand)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = cand;
        }

        /* Advance to the next integer candidate. */
        ++candidate;
    }

    return cache[n - 1];
}
