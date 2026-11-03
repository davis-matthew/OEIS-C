/*
 * A139653  Primes of the form x^2 + 253*y^2.
 * https://oeis.org/A139653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime that can be written as x² + 253·y² (offset
 *            1)
 * Range:     u8 none, u16 n <= 777, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 9538 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isqrt_seq.h"
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

/* Allowed residues modulo 1012 (from the OEIS note). */
static const idx_t residues[] = {
    1,  9, 25, 49, 81, 93, 133, 141, 169, 177,
    185, 213, 225, 257, 265, 269, 289, 301, 317,
    353, 357, 361, 377, 397, 441, 445, 449, 485,
    489, 509, 533, 537, 553, 565, 577, 581, 625,
    653, 669, 685, 729, 749, 785, 817, 829, 837,
    841, 905, 929, 933, 949, 961, 969, 993, 1005
};
#define RESIDUE_COUNT (sizeof(residues)/sizeof(residues[0]))

/* Cache for already found terms. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static idx_t  last_checked = 1;   /* last tested candidate (inclusive) */

/* Check whether a prime p can be written as x² + 253·y². */
static bool has_representation(seq_t p)
{
    const seq_t d = (seq_t)253;
    if (p < (seq_t)5) return false;

    seq_t max_y = isqrt_seq(p / d);

    for (seq_t y = 0; y <= max_y; ++y) {
        seq_t yy = y * y;
        seq_t r  = p - d * yy;
        seq_t sq = isqrt_seq(r);
        if (sq * sq == r) return true;
    }
    return false;
}

/* The sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined offset 0 */

    /* Extend cache until it contains at least n entries. */
    while (cache_len < n) {
        idx_t cand = last_checked;      /* start searching after last_checked */
        for (;;) {
            ++cand;

            /* Skip even numbers > 2. */
            if (cand > 2 && (cand & 1U) == 0) continue;

            seq_t p = (seq_t)cand;

            /* Quick congruence test: p mod 1012 must be in residues. */
            idx_t rem = (idx_t)(p % (idx_t)1012);
            bool ok = false;
            for (size_t i = 0; i < RESIDUE_COUNT; ++i) {
                if (rem == residues[i]) { ok = true; break; }
            }
            if (!ok) continue;

            /* Test primality. */
            if (!is_prime_big(p)) continue;

            /* Test representation. */
            if (has_representation(p)) {
                /* Allocate more space if needed. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) return (seq_t)0;  /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = p;
                last_checked = cand;
                break;  /* proceed to next required term */
            }
        }
    }

    return cache[n - 1];
}
