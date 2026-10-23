/*
 * A162422  Numbers with at least 2 different numbers of digits among their
 *          prime factors.
 * https://oeis.org/A162422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates sequence A162422: Numbers with at least two different
   numbers of digits among their prime factors. It builds the terms on demand
   using incremental sieve and trial division. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_count.h"

static seq_t *seq_vals = NULL;      /* array of sequence values */
static size_t seq_cap = 0, seq_len = 0;
static uint64_t tested = 1;         /* last integer tested */

static uint64_t *prime_list = NULL; /* list of primes */
static size_t prime_cap = 0, prime_count = 0;
static uint64_t prime_limit = 1;    /* largest prime generated */

/* ----- auxiliary functions for dynamic memory ----- */
static void grow_seq_cap(void) {
    if (seq_cap == 0) {
        seq_cap = 64;
        seq_vals = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    } else {
        seq_cap *= 2;
        seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
    }
}

static void grow_prime_cap(void) {
    if (prime_cap == 0) {
        prime_cap = 64;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    } else {
        prime_cap *= 2;
        prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
    }
}

/* ----- prime generation up to a given limit ----- */
static void ensure_primes_up_to(uint64_t limit) {
    if (limit <= prime_limit) return;

    for (uint64_t cand = prime_limit + 1; cand <= limit; ++cand) {
        bool isprime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            if (prime_count == prime_cap) grow_prime_cap();
            prime_list[prime_count++] = cand;
        }
    }
    prime_limit = limit;
}

/* ----- test if a number has at least two different digit lengths
   among its prime factors. ----- */
static bool has_two_digit_lengths(uint64_t n) {
    if (n < 4) return false; /* 4 = 2^2, all 1‑digit factors */
    uint64_t tmp = n;

    int len1 = 0, len2 = 0;

    /* ensure we have primes up to sqrt(n) */
    ensure_primes_up_to(tmp);

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            int d = digit_count(p);
            if (len1 == 0)
                len1 = d;
            else if (d != len1 && len2 == 0) {
                len2 = d;
                if (len2 != len1) return true;
            } else if (d != len1 && d != len2) return true;

            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {
        int d = digit_count(tmp);
        if (len1 == 0)
            len1 = d;
        else if (d != len1 && len2 == 0) {
            len2 = d;
            if (len2 != len1) return true;
        } else if (d != len1 && d != len2) return true;
    }
    return len2 != 0;
}

/* ----- main sequence function ----- */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, return 0 */

    /* initialize storage if needed */
    if (seq_vals == NULL) grow_seq_cap();

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        ++tested; /* examine next integer */
        if (has_two_digit_lengths(tested)) {
            if (seq_len == seq_cap) grow_seq_cap();
            seq_vals[seq_len++] = (seq_t)tested;
        }
    }
    return seq_vals[n - 1];
}
