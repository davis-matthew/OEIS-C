/*
 * A080360  a(n) is the largest positive integer x such that the number of
 *          unitary-prime-divisors of x! equals n. Same as the largest
 *          positive integer x such that the number of primes in (x/2,x]
 *          equals n.
 * https://oeis.org/A080360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Max{x ; π(x) – π(x/2) = n} (n ≥ 1)
 * Range:     u8 none, u16 n <= 3029, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by a one‑time sieve and prefix sums for π(x). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIMIT     400000U            /* upper bound for x to cover n ≤ 10 000   */
#define MAX_INDEX 200000U            /* maximum n that we pre‑compute           */

static bool        *prime_flags = NULL;  /* primality flags 0..LIMIT          */
static seq_t       *pi_arr      = NULL;  /* π(x) for 0..LIMIT                */
static seq_t       *ans_arr     = NULL;  /* ans_arr[n] = largest x with diff n */
static bool         initialized = false;   /* one‑time init flag                 */

/* one‑time initialization: sieve, prefix sums, answer table */
static void init_sequence(void)
{
    if (initialized) return;
    initialized = true;

    /* Allocate arrays -------------------------------------------------- */
    prime_flags = (bool*)calloc(LIMIT + 1, sizeof(bool));
    pi_arr      = (seq_t*)calloc(LIMIT + 1, sizeof(seq_t));
    ans_arr     = (seq_t*)calloc(MAX_INDEX + 1, sizeof(seq_t));

    /* Sieve of Eratosthenes ------------------------------------------- */
    for (uint64_t i = 2; i <= LIMIT; ++i) prime_flags[i] = true;
    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (prime_flags[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                prime_flags[j] = false;
        }
    }

    /* Compute π(x) ----------------------------------------------------- */
    pi_arr[0] = 0;
    for (uint64_t i = 1; i <= LIMIT; ++i)
        pi_arr[i] = pi_arr[i - 1] + (prime_flags[i] ? (seq_t)1 : (seq_t)0);

    /* Build answer table for each difference --------------------------- */
    for (uint64_t x = 2; x <= LIMIT; ++x) {
        seq_t diff = pi_arr[x] - pi_arr[x / 2];
        if (diff == 0 || diff > (seq_t)MAX_INDEX) continue;
        idx_t d = (idx_t)diff;
        /* store the largest x for this diff */
        if (ans_arr[d] == 0 || ans_arr[d] < x)
            ans_arr[d] = (seq_t)x;
    }
}

/* --------------------------------------------------------------------- */
/*  Public sequence function                                             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n==0 */

    init_sequence();                     /* ensure data are ready */

    if (n <= (idx_t)MAX_INDEX)
        return ans_arr[(idx_t)n];
    return 0;                            /* out of pre‑computed range   */
}
