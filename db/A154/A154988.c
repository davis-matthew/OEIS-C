/*
 * A154988  Odd numbers that are the right-angle adjacent side in more than
 *          one primitive Pythagorean triple with prime hypotenuse.
 * https://oeis.org/A154988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 32, u32 n <= 6000, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Validated: standard, pass, 69 terms through n = 5618 (2026-11-03)
 * Helpers:   gcd_idx, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A154988 by enumerating odd
   integers and testing for more than one primitive
   Pythagorean triple with prime hypotenuse. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd_idx.h"
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

/* Check whether an odd integer a is the adjacent side of
   more than one primitive Pythagorean triple whose
   hypotenuse is prime. */
static bool has_multiple_prime_hypotenuse(uint64_t a)
{
    uint64_t count = 0;
    for (uint64_t d1 = 1; d1 <= a / d1; d1 += 2) {    /* odd divisors only */
        if (a % d1 != 0)
            continue;
        uint64_t d2 = a / d1;
        if (d1 >= d2)               /* we have considered all unordered pairs */
            break;
        if (gcd_idx(d1, d2) != 1) /* gcd(m,n)=1 ⇔ gcd(d1,d2)=1 */
            continue;
        uint64_t m = (d1 + d2) >> 1; /* (d1+d2)/2 */
        uint64_t n = (d2 - d1) >> 1; /* (d2-d1)/2 */
        seq_t hyp = (seq_t)m * m + (seq_t)n * n;
        if (is_prime_big(hyp)) {
            ++count;
            if (count >= 2)
                return true;
        }
    }
    return false;
}

/* Static cache for the sequence */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_a   = 1;   /* next odd integer to test */

/* OEIS entry */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; 0 is unused */
        return 0;

    /* Ensure the cache contains at least n terms */
    while (cache_len < (size_t)n) {
        /* Test the current odd integer */
        if (has_multiple_prime_hypotenuse(next_a)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = (seq_t)next_a;
        }

        /* Advance to the next odd integer */
        if (next_a > UINT64_MAX - 2)        /* avoid overflow */
            break;
        next_a += 2;
    }

    /* Return the requested term */
    return cache[(size_t)n - 1];
}
