/*
 * A155058  Primes with odd largest digit.
 * https://oeis.org/A155058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 73 terms through n = 1952 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: OEIS A155058 – primes whose largest decimal digit is odd */
/* The code generates the primes on demand; it keeps an internal list
   of already found terms so that subsequent calls are efficient. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Return whether the largest decimal digit of x is odd */
static bool odd_largest_digit(uint64_t x)
{
    unsigned maxd = 0;
    while (x > 0) {
        unsigned d = x % 10;
        if (d > maxd) maxd = d;
        x /= 10;
    }
    return (maxd & 1U) != 0;
}

static seq_t *prime_cache = NULL;   /* dynamic array of sequence values */
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Grow the dynamic array to hold at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
        prime_cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑based; ignore behaviour for n < 1, as required */
    if (n < (idx_t)1) return (seq_t)0;  /* a safeguard, not used by tests */

    /* Extend the cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        /* Find the next candidate prime */
        uint64_t candidate;
        if (cache_len == 0) {
            candidate = 3;                 /* first prime with odd largest digit */
        } else {
            candidate = (uint64_t)prime_cache[cache_len - 1] + 2;
        }

        /* Advance to the next odd number; skip even candidates */
        for (;;) {
            /* Skip even numbers (except 2 which is not in the list) */
            if (candidate % 2 != 0) {
                seq_t cand_seq = (seq_t)candidate;
                if (is_prime_big(cand_seq) && odd_largest_digit(candidate)) {
                    /* Record the prime */
                    ensure_cache_capacity(cache_len + 1);
                    prime_cache[cache_len++] = cand_seq;
                    break;              /* found next term */
                }
            }
            candidate += 2;               /* next odd number */
        }
    }

    /* Return the requested term */
    return prime_cache[(size_t)n - 1];
}
