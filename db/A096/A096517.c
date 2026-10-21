/*
 * A096517  Solutions to A096509[x]=6; number of prime-powers [including
 *          primes] in the neighborhood of x with Ceiling[Log[x]] radius
 *          equals 6.
 * https://oeis.org/A096517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer x such that the number of prime‑powers
 *            (including primes) in the neighbourhood [x‑⌈log_e(x)⌉,
 *            x+⌈log_e(x)⌉] equals 6. The function a(n) returns the n‑th such
 *            integer. The implementation sieves all prime‑powers up to a
 *            fixed limit (10,000,000) and searches successive x unt...
 * Range:     u8 none, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define PREALLOC 1024
#define LIMIT 10000000  /* assumed large enough for n up to ~10000 */

static bool *prime_power = NULL;          /* prime‑power flag array */
static size_t prime_power_cap = 0;        /* size of prime_power array */
static uint64_t e_power[65];              /* ceil(e^k) thresholds */
static bool init_done = false;            /* flag */
static idx_t search_start = 2;            /* next number to test */
static seq_t *solutions = NULL;           /* array of found solutions */
static size_t sol_size = 0;               /* number of solutions stored */
static size_t sol_cap = 0;                /* capacity of solutions array */

/* --- helper: build e_power thresholds (once) ------------------ */
static void build_thresholds(void) {
    e_power[0] = 1;
    for (int k = 1; k < 65; ++k) {
        /* use double to compute e^k, then cast to uint64_t */
        e_power[k] = (uint64_t)ceil(exp((double)k));
    }
}

/* --- helper: compute ceil(log_e(x)) using thresholds -------- */
static idx_t ceil_log_e(idx_t x) {
    /* smallest r such that e_power[r] > x */
    idx_t r = 1;
    while (r < 65 && e_power[r] <= x) ++r;
    return r;
}

/* --- helper: sieve all primes up to LIMIT and mark prime‑powers */
static void build_prime_power_sieve(void) {
    /* Allocate prime_power array */
    prime_power_cap = LIMIT + 1;
    prime_power = (bool *)malloc(prime_power_cap * sizeof(bool));
    if (!prime_power) exit(1);
    for (size_t i = 0; i < prime_power_cap; ++i) prime_power[i] = false;

    /* Simple sieve of Eratosthenes */
    bool *is_prime = (bool *)malloc(prime_power_cap * sizeof(bool));
    if (!is_prime) exit(1);
    for (size_t i = 0; i < prime_power_cap; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i < prime_power_cap; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j < prime_power_cap; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* Mark prime‑powers */
    for (size_t p = 2; p < prime_power_cap; ++p) {
        if (!is_prime[p]) continue;
        /* powers of prime p */
        uint64_t val = p;
        while (val < prime_power_cap) {
            prime_power[val] = true;
            if (val > (uint64_t)(prime_power_cap - 1) / p) break;
            val *= p;
        }
    }
    free(is_prime);
}

/* --- helper: ensure solutions array capacity ------------------ */
static void ensure_sol_cap(void) {
    if (sol_size >= sol_cap) {
        size_t new_cap = sol_cap == 0 ? PREALLOC : sol_cap * 2;
        seq_t *new_arr = (seq_t *)realloc(solutions, new_cap * sizeof(seq_t));
        if (!new_arr) exit(1);
        solutions = new_arr;
        sol_cap = new_cap;
    }
}

/* --- main function requested --------------------------------- */
seq_t a(idx_t n) {
    if (!init_done) {
        build_thresholds();
        build_prime_power_sieve();
        init_done = true;
    }

    /* generate solutions until we have at least n */
    while (sol_size < n) {
        idx_t x = search_start;
        ++search_start;

        idx_t r = ceil_log_e(x);
        idx_t left = x > r ? x - r : 1;
        idx_t right = x + r;
        if (right >= prime_power_cap) right = prime_power_cap - 1;

        size_t count = 0;
        for (idx_t y = left; y <= right; ++y) {
            if (prime_power[y]) ++count;
        }

        if (count == 6) {
            ensure_sol_cap();
            solutions[sol_size++] = (seq_t)x;
        }
    }

    return solutions[n - 1];
}
