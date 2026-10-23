/*
 * A184830  a(n) = largest k such that A000961(n+1) = A000961(n) + (A000961(n)
 *          mod k), or 0 if no such k exists.
 * https://oeis.org/A184830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest k such that A000961(n+1) = A000961(n) +
 *            (A000961(n) mod k), or 0 when no such k exists
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 9790, u64 n <= 9790,
 *            u128 n <= 9790, big any n
 * Helpers:   compare_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence is computed for any n ≥ 1 using a pre‑computed list of prime powers.
    A small, static sieve up to a fixed limit (2 000 000) guarantees that
    the first 10 000 terms are available. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/compare_seq_t.h"

static seq_t *prime_powers = NULL;   /* 0‑indexed array of A000961 values   */
static size_t pp_size  = 0;          /* number of elements stored            */
static size_t pp_cap   = 0;          /* current capacity of the array         */
static bool init_done = false;       /* flag to prevent re‑initialisation     */

/* Comparator for qsort over seq_t values */

/* Grow the prime‑powers array to accommodate at least `need` elements */
static void ensure_capacity_pp(size_t need)
{
    if (pp_cap >= need) return;
    size_t new_cap = pp_cap ? pp_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(prime_powers, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    prime_powers = tmp;
    pp_cap = new_cap;
}

/* Initialise the global data structures: generate prime powers up to LIMIT. */
static void init_sequence(void)
{
    if (init_done) return;
    init_done = true;

    /* Choose a limit that guarantees enough prime powers for the
       required range of n (≥10 000).  2 000 000 yields >145 000 prime powers. */
    const idx_t LIMIT = 2000000ULL;

    /* Allocate initial space for prime powers. */
    pp_cap   = 8192;
    prime_powers = (seq_t *)malloc(pp_cap * sizeof(seq_t));
    if (!prime_powers) exit(EXIT_FAILURE);

    /* Insert the first element of A000961 (value 1). */
    ensure_capacity_pp(1);
    prime_powers[0] = (seq_t)1;
    pp_size = 1;

    /* Very small sieve of Eratosthenes to find primes up to LIMIT. */
    bool *prime_flag = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!prime_flag) exit(EXIT_FAILURE);
    memset(prime_flag, 0, (LIMIT + 1) * sizeof(bool));

    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (!prime_flag[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                prime_flag[j] = true;

    /* For each prime, generate its powers ≤ LIMIT and store them. */
    for (idx_t p = 2; p <= LIMIT; ++p)
    {
        if (!prime_flag[p]) {
            unsigned long long power = p;
            while (power <= LIMIT) {
                ensure_capacity_pp(pp_size + 1);
                prime_powers[pp_size++] = (seq_t)power;
                if (power > LIMIT / p) break;            /* avoid overflow */
                power *= p;
            }
        }
    }

    free(prime_flag);

    /* Sort the collected prime powers to obtain A000961 in ascending order. */
    qsort(prime_powers, pp_size, sizeof(seq_t), compare_seq_t);
}

/* Main OEIS function as required. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Build the table once. */
    init_sequence();

    /* Ensure we have at least the (n+1)-th element. */
    if ((size_t)n + 1 > pp_size) return (seq_t)0;

    /* Retrieve A000961(n) and A000961(n+1). */
    seq_t pn_seq = prime_powers[n - 1];
    seq_t pn1_seq = prime_powers[n];

    /* Work in unsigned long long for the arithmetic needed. */
    unsigned long long pn   = (unsigned long long)pn_seq;
    unsigned long long pn1  = (unsigned long long)pn1_seq;

    /* Compute the difference d = A000961(n+1) – A000961(n). */
    unsigned long long d = pn1 - pn;
    if (d == 0) return (seq_t)0;            /* happens for n = 1 only */

    /* base = A000961(n) – d  must be positive and > d for a valid k. */
    unsigned long long base = pn - d;
    if (base <= d) return (seq_t)0;

    /* Find the largest divisor of ‘base’ that exceeds d. */
    unsigned long long best = 0;
    for (unsigned long long k = 1; k * k <= base; ++k) {
        if (base % k == 0) {
            unsigned long long k1 = k;
            unsigned long long k2 = base / k;
            if (k1 > d && k1 > best) best = k1;
            if (k2 > d && k2 > best) best = k2;
        }
    }

    return (seq_t)best;
}
