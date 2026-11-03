/*
 * A216741  Primes p such that x^20 = -2 has no solution mod p.
 * https://oeis.org/A216741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 944 (2026-11-01)
 * Helpers:   gcd_u64, is_prime_seq, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A216741: primes p for which x^20 = -2 has no solution modulo p
   The test is: (-2)^((p-1)/gcd(p-1,20)) ≠ 1 (mod p) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd_u64.h"
#include "../utils/is_prime_seq.h"
#include "../utils/powmod.h"

/* Dynamic array helpers for uint64_t (primes that satisfy the condition) */
static void ensure_capacity_uint64(uint64_t **array, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t newcap = (*cap == 0) ? 8 : *cap * 2;
        while (newcap < needed) newcap *= 2;
        uint64_t *tmp = (uint64_t *)realloc(*array, newcap * sizeof(uint64_t));
        if (!tmp) {
            /* Allocation failed; terminate the program - this is fatal */
            abort();
        }
        *array = tmp;
        *cap = newcap;
    }
}

/* Global state for on‑demand generation */
static uint64_t *good = NULL;    /* primes satisfying the property */
static size_t   good_cnt  = 0;
static size_t   good_cap  = 0;
static uint64_t next_candidate = 3;   /* next odd number to test */

/* Ensure that we have at least `needed` good primes in the array. */
static void ensure_good_count(size_t needed)
{
    while (good_cnt < needed) {
        uint64_t p = next_candidate;
        /* test primality */
        if (is_prime_seq((seq_t)p)) {
            /* compute condition: (-2)^((p-1)/gcd(p-1,20)) mod p */
            uint64_t g = gcd_u64(p - 1, 20ULL);
            uint64_t exp = (p - 1) / g;
            seq_t r = powmod((seq_t)(p - 2ULL), exp, (seq_t)p);
            if (r != 1) {
                /* store good prime */
                ensure_capacity_uint64(&good, &good_cap, good_cnt + 1);
                good[good_cnt++] = p;
            }
        }
        next_candidate += 2;   /* skip even numbers */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined offset; safe */
    size_t need = (size_t)n;
    ensure_good_count(need);
    return (seq_t)good[need - 1];
}
