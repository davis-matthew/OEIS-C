/*
 * A106711  Smaller prime of the set of two consecutive primes each of whose
 *          sum of digits is prime.
 * https://oeis.org/A106711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the smaller prime of a pair of consecutive primes whose
 *            * digit sums are prime. a(1) = 2, a(2) = 3, a(3) = 5, a(4) = 7,
 *            a(5) = 23,
 * Range:     u8 n <= 18, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Ensure an array of uint64_t is large enough to hold indexes [0..min_needed] */
static void ensure_uint64_array(uint64_t **arr, size_t *cap, size_t min_needed)
{
    if (*cap <= min_needed) {
        size_t newcap = (*cap) ? (*cap) * 2 : 64;
        if (newcap <= min_needed) newcap = min_needed + 1;
        *arr = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
        *cap = newcap;
    }
}

/* Generate the next prime after last */
static uint64_t generate_next_prime(uint64_t last,
                                    uint64_t **primes, size_t *cnt, size_t *cap)
{
    uint64_t candidate = last + 1;
    if (candidate <= 2) candidate = 2;
    if (candidate > 2 && (candidate & 1) == 0) ++candidate;  /* start from odd */

    for (;;) {
        bool prime = true;
        for (size_t i = 0; i < *cnt; ++i) {
            uint64_t p = (*primes)[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { prime = false; break; }
        }
        if (prime) {
            ensure_uint64_array(primes, cap, *cnt);
            (*primes)[*cnt] = candidate;
            ++(*cnt);
            return candidate;
        }
        candidate += 2;
    }
}

static seq_t *terms = NULL;            /* array of sequence terms */
static size_t terms_len = 0, terms_cap = 0;

static uint64_t *prime_list = NULL;    /* list of primes found so far */
static size_t prime_cnt = 0, prime_cap = 0;
static uint64_t last_prime = 0;        /* last prime returned by generate_next_prime */
static uint64_t prev_prime = 0;        /* previous prime in the list */
static bool prev_sod_prime = false;    /* whether digit sum of prev_prime is prime */
static bool initialized = false;       /* one‑time init flag */

/* test whether the sum of decimal digits of p is prime */
static bool digit_sum_is_prime(uint64_t p) {
    uint64_t s = 0;
    while (p) {
        s += p % 10;
        p /= 10;
    }
    return is_prime_u64(s);
}

/* append a new term to the dynamic array */
static void append_term(uint64_t v) {
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap == 0 ? 16 : terms_cap * 2;
        seq_t *tmp = mem_resize(seq_t, terms, new_cap);
        if (!tmp) exit(1);
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = (seq_t)v;
}

seq_t a(idx_t n) {
    if (!initialized) {
        /* nothing special to initialise beyond static variables */
        initialized = true;
    }
    /* generate terms until we have at least n of them */
    while (terms_len < n) {
        uint64_t next = generate_next_prime(last_prime, &prime_list,
                                            &prime_cnt, &prime_cap);
        bool sod = digit_sum_is_prime(next);
        if (prime_cnt >= 2) {            /* we have a previous prime to pair with */
            if (prev_sod_prime && sod) {
                append_term(prev_prime);
            }
        }
        /* shift window for next iteration */
        prev_prime = next;
        prev_sod_prime = sod;
        last_prime = next;
    }
    return terms[n - 1];
}
