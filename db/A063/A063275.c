/*
 * A063275  Numbers that require three powerful numbers (definition 1) to sum
 *          to them.
 * https://oeis.org/A063275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers that require three powerful numbers (definition
 *            1) to sum to them
 * Range:     u8 n <= 91, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A non‑trivial solver that builds the set of powerful numbers up to a fixed limit,
   precomputes which integers can be expressed as a sum of two powerful numbers,
   and then generates the required terms on demand. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/append_candidate.h"

static bool initialized = false;
static seq_t *terms = NULL;      /* dynamic array of found terms */
static size_t terms_len = 0;     /* number of terms stored */
static size_t terms_cap = 0;     /* capacity of terms array */

/* Tables for powerful numbers and their sums */
static uint8_t *is_powerful = NULL;   /* 1 if i is powerful */
static uint8_t *rep_two    = NULL;   /* 1 if i can be expressed as a sum of two powerful numbers */

/* List of powerful numbers */
static idx_t *powers = NULL;
static size_t powers_len = 0;
static size_t powers_cap = 0;

static const idx_t LIMIT = 500000ull; /* should be large enough for 10 000 terms */

/* Append a new powerful number to the powers list. */
static void push_power(idx_t v)
{
    if (powers_len == powers_cap) {
        powers_cap = powers_cap ? powers_cap * 2 : 8;
        powers = (idx_t *)realloc(powers, powers_cap * sizeof(idx_t));
    }
    powers[powers_len++] = v;
}

/* Initialize all helper arrays and precompute powerful numbers. */
static void init(void)
{
    /* Allocate flag arrays */
    is_powerful = (uint8_t *)calloc(LIMIT + 1, sizeof(uint8_t));
    rep_two    = (uint8_t *)calloc(LIMIT + 1, sizeof(uint8_t));

    /* Allocate space for powerful numbers list */
    powers = (idx_t *)malloc(8 * sizeof(idx_t));
    powers_cap = 8;
    powers_len = 0;

    /* Smallest prime factor array */
    idx_t *spf = (idx_t *)malloc((LIMIT + 1) * sizeof(idx_t));
    memset(spf, 0, (LIMIT + 1) * sizeof(idx_t));

    /* Prime list for linear sieve */
    uint32_t *prime_arr = (uint32_t *)malloc((LIMIT / 10 + 1) * sizeof(uint32_t));
    size_t p_cnt = 0;
    size_t p_cap = LIMIT / 10 + 1;

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (p_cnt == p_cap) {
                p_cap *= 2;
                prime_arr = (uint32_t *)realloc(prime_arr, p_cap * sizeof(uint32_t));
            }
            prime_arr[p_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < p_cnt; ++j) {
            uint32_t p = prime_arr[j];
            idx_t prod = i * p;
            if (prod > LIMIT) break;
            spf[prod] = p;
            if (p == spf[i]) break;
        }
    }

    /* Determine powerful numbers up to LIMIT */
    for (idx_t n = 1; n <= LIMIT; ++n) {
        if (n == 1) {
            is_powerful[n] = 1;
            push_power(n);
            continue;
        }
        idx_t tmp = n;
        bool ok = true;
        while (tmp > 1) {
            idx_t p = spf[tmp];
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (exp < 2) {
                ok = false;
                break;
            }
        }
        if (ok) {
            is_powerful[n] = 1;
            push_power(n);
        }
    }

    /* Pre‑compute sums of two powerful numbers */
    for (size_t i = 0; i < powers_len; ++i) {
        idx_t a = powers[i];
        for (size_t j = i; j < powers_len; ++j) {
            idx_t b = powers[j];
            idx_t sum = a + b;
            if (sum > LIMIT) break;
            rep_two[sum] = 1;
        }
    }

    free(spf);
    free(prime_arr);
    free(terms);            /* in case of re‑initialisation, free old */
    terms = NULL;
    terms_len = 0;
    terms_cap = 0;
    initialized = true;
}

/* Ensure that we have computed at least n terms in the global sequence. */
static void ensure_terms(idx_t n)
{
    static idx_t candidate = 0;
    while (terms_len < n) {
        ++candidate;
        if (candidate > LIMIT) {
            /* We hit the artificial limit; further terms cannot be computed. */
            break;
        }

        if (is_powerful[candidate] || rep_two[candidate])
            continue; /* requires less than three powerful numbers */

        /* Check if candidate can be written as sum of three powerful numbers.
           Equivalent to: exists p in powerful_numbers such that
           rep_two[candidate - p] == 1. */
        bool ok = false;
        for (size_t i = 0; i < powers_len && powers[i] <= candidate; ++i) {
            idx_t p = powers[i];
            if (rep_two[candidate - p]) {
                ok = true;
                break;
            }
        }
        if (ok) {
            /* Store the term in the dynamic array */
            append_candidate(&terms, &terms_len, &terms_cap, (seq_t)candidate);
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!initialized)
        init();
    if (n == 0)            /* index out of bounds */
        return 0;
    ensure_terms(n);
    return terms[n - 1];
}
