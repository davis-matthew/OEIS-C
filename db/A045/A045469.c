/*
 * A045469  Primes congruent to {1, 4} mod 7.
 * https://oeis.org/A045469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 991 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: returns the n-th prime congruent to 1 or 4 (mod 7), offset 1. */
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

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;
    static size_t primes_size = 0;
    static size_t primes_cap = 0;
    static seq_t last_checked = 0;

    /* initialize the first element */
    if (primes_size == 0) {
        primes_cap = 16;
        primes = mem_new(seq_t, primes_cap);
        primes[0] = (seq_t)11;
        primes_size = 1;
        last_checked = (seq_t)11;
    }

    /* if already computed, return directly */
    if ((size_t)n <= primes_size) {
        return primes[(size_t)n - 1];
    }

    /* otherwise generate until we have at least n primes */
    while (primes_size < (size_t)n) {
        seq_t cand = last_checked + (seq_t)2;   /* next odd candidate */
        for (;;) {
            if ((cand % (seq_t)7 == (seq_t)1) || (cand % (seq_t)7 == (seq_t)4)) {
                if (is_prime_big(cand)) {
                    break;       /* found the next required prime */
                }
            }
            cand += (seq_t)2;   /* keep the candidate odd */
        }

        /* grow array if necessary */
        if (primes_size >= primes_cap) {
            primes_cap = primes_cap * 2 + 1;
            primes = mem_resize(seq_t, primes, primes_cap);
        }

        primes[primes_size++] = cand;
        last_checked = cand;
    }

    return primes[(size_t)n - 1];
}
