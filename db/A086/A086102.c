/*
 * A086102  Concatenation of last two digits of p(n) and first two digits of
 *          p(n+1) is prime; values of such n in the sequence.
 * https://oeis.org/A086102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate primes sequentially and collect the indices n
   such that the concatenation of the last two digits of p(n) and the
   first two digits of p(n+1) is prime.  Only pairs where both primes
   have at least two decimal digits are considered.  The function
   returns a(n) for any n ≥ 1, with memoisation. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_fast.h"

/* ---------- small prime test (≤ 2^31) ---------- */
static bool is_prime_small(uint32_t x) {
    if (x < 2)             return false;
    if (x % 2 == 0)        return x == 2;
    for (uint32_t d = 3; d * d <= x; d += 2)
        if (x % d == 0)
            return false;
    return true;
}

/* ---------- support: number of decimal digits ---------- */
static int num_digits(uint32_t x) {
    int d = 0;
    do { ++d; x /= 10; } while (x);
    return d;
}

/* ---------- first two digits of a number with ≥2 digits ---------- */
static uint32_t first_two_digits(uint32_t x) {
    int d = num_digits(x);
    if (d <= 2) return x;               /* x has 2 digits: return x */
    /* For d ≥ 3: divide by 10^(d-2) */
    uint32_t div = 1;
    for (int i = 0; i < d - 2; ++i) div *= 10;
    return x / div;
}

/* ---------- dynamic arrays for primes and matches ---------- */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0, prime_cap = 0;
static uint32_t *match_list = NULL;
static size_t match_count = 0, match_cap = 0;

/* ---------- ensure capacity for a dynamic array ---------- */
static void ensure_cap(uint32_t **arr, size_t *len, size_t *cap, size_t need) {
    if (need > *cap) {
        size_t newcap = (*cap == 0) ? 16 : *cap * 2;
        while (newcap < need) newcap *= 2;
        uint32_t *tmp = (uint32_t *)realloc(*arr, newcap * sizeof(uint32_t));
        *arr = tmp;
        *cap = newcap;
    }
}

/* ---------- generate the next prime after the last stored one ---------- */
static uint32_t generate_next_prime(void) {
    uint32_t cand = prime_list[prime_count - 1] + 1;
    for (;; ++cand) {
        if (is_prime_fast(cand, prime_list, prime_count))
            break;
    }
    ensure_cap(&prime_list, &prime_count, &prime_cap, prime_count + 1);
    prime_list[prime_count++] = cand;
    return cand;
}

/* ---------- ensure we have at least n matches stored ---------- */
static void ensure_matches(idx_t n) {
    /* bootstrap: ensure prime_list starts with 2 if empty */
    if (prime_count == 0) {
        ensure_cap(&prime_list, &prime_count, &prime_cap, 1);
        prime_list[prime_count++] = 2;
    }

    while (match_count < (size_t)n) {
        /* generate next prime */
        uint32_t next_prime = generate_next_prime();
        if (prime_count >= 2) {
            uint32_t prev_prime = prime_list[prime_count - 2];

            /* require both primes to have at least two decimal digits */
            if (prev_prime >= 10 && next_prime >= 10) {
                uint32_t suffix = prev_prime % 100;
                uint32_t prefix = first_two_digits(next_prime);
                uint32_t concat = suffix * 100 + prefix;
                if (is_prime_small(concat)) {
                    /* n equals the index of prev_prime (1‑based) */
                    uint32_t idx = (uint32_t)(prime_count - 1);
                    ensure_cap(&match_list, &match_count, &match_cap, match_count + 1);
                    match_list[match_count++] = idx;
                }
            }
        }
    }
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for zero */
    ensure_matches(n);
    return (seq_t)match_list[(size_t)n - 1];
}
