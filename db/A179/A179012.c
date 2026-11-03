/*
 * A179012  Primes that are the sum of three consecutive composite odd
 *          numbers.
 * https://oeis.org/A179012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 952, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 977 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th prime that is the sum of three consecutive composite odd numbers.
 * Computation proceeds by scanning odd numbers, collecting composite odds in a sliding window,
 * summing them, and testing the sum for primality. */

#include <stddef.h>
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* state kept between calls to build the sequence incrementally */
static seq_t c1 = 0, c2 = 0, c3 = 0;
static idx_t seen_composite = 0;
static idx_t next_odd = 3;  /* the next odd number to examine */

/* Ensure at least `target` terms have been generated. */
static void ensure_cache(idx_t target)
{
    /* fast exit if already have enough terms */
    if (cache_len >= (size_t)target)
        return;

    while (cache_len < (size_t)target) {
        /* find next odd composite number */
        while (true) {
            if (!is_prime_big((seq_t)next_odd))
                break;          /* next_odd is composite */
            next_odd += 2;       /* skip to the next odd */
        }

        /* process this composite odd */
        seen_composite++;
        if (seen_composite == 1) {
            c1 = next_odd;
        } else if (seen_composite == 2) {
            c2 = next_odd;
        } else {
            c3 = next_odd;
            seq_t sum = c1 + c2 + c3;
            if (is_prime_big(sum)) {
                grow_seq(&cache, &cache_cap, cache_len + 1);
                cache[cache_len++] = sum;
            }
            /* shift window for next triple */
            c1 = c2;
            c2 = c3;
        }

        /* prepare for next odd number */
        next_odd += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for offset 1, but return 0 for safety */
        return 0;
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
