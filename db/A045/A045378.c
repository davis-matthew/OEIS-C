/*
 * A045378  Primes congruent to {2, 4} mod 5.
 * https://oeis.org/A045378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 995 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Incrementally search integer by integer, testing each
   candidate with is_prime_big.  Collect those primes that are
   congruent to 2 or 4 modulo 5.  Uses static arrays to memoise
   already found terms so that calls may appear in any order. */

#include <stdbool.h>
#include <stdlib.h>
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

/* Cached filtered primes (≡ 2 or 4 mod 5). */
static seq_t *filtered = NULL;
static size_t filt_count = 0, filt_cap = 0;

/* Next integer to be tested for primality. */
static seq_t next_candidate = 2;

/* Ensure capacity for the next element of filtered. */
static void ensure_filter_capacity(void)
{
    if (filt_cap <= filt_count) {
        size_t new_cap = filt_cap ? filt_cap * 2 : 16;
        seq_t *tmp = mem_resize(seq_t, filtered, new_cap);
        if (!tmp) abort();
        filtered = tmp;
        filt_cap = new_cap;
    }
}

/* Return the n‑th prime congruent to 2 or 4 (mod 5). */
seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first term. */
    while (filt_count < n) {
        if (is_prime_big(next_candidate)) {
            if ((next_candidate % 5 == 2) || (next_candidate % 5 == 4)) {
                ensure_filter_capacity();
                filtered[filt_count++] = next_candidate;
            }
        }
        ++next_candidate;
    }
    return filtered[n - 1];
}
