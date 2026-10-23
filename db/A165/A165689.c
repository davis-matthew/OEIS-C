/*
 * A165689  Numbers n such that pi(n) = (1/10)*n.
 * https://oeis.org/A165689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = the nth integer n > 0 such that π(n) = n/10
 * Range:     u8 none, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Notes: only multiples of 10 can satisfy the condition; the sequence
   is found by sieving primes up to the necessary bound, storing a
   prefix prime‑count array, then scanning candidates n=10*k. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* ----- static memoisation state ----- */
static idx_t      sieve_limit   = 0;       /* highest n that has been sieved */
static unsigned char *is_prime = NULL;     /* 0 = composite, 1 = prime   */
static seq_t *pi_arr          = NULL;     /* cumulative prime counts    */
static seq_t *solutions       = NULL;     /* found a(k) values          */
static size_t sol_len         = 0;         /* number of solutions found  */
static size_t sol_cap         = 0;         /* capacity of solutions array */
static idx_t  next_k          = 1;         /* next candidate k (n = 10*k) */

/* ----- helper: resize and rebuild the sieve up to at least new_limit ------- */
static void extend_sieve(idx_t new_limit)
{
    /* free old structures */
    if (is_prime) free(is_prime);
    if (pi_arr)   free(pi_arr);

    /* allocate prime flag array */
    is_prime = (unsigned char*)calloc((size_t)(new_limit + 1), 1);
    if (!is_prime) abort();

    /* mark non‑primes via Eratosthenes */
    if (new_limit >= 0) is_prime[0] = 0;
    if (new_limit >= 1) is_prime[1] = 0;
    for (idx_t i = 2; i <= new_limit; ++i) is_prime[i] = 1;

    for (idx_t p = 2; p * p <= new_limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= new_limit; j += p)
                is_prime[j] = 0;
        }
    }

    /* build cumulative prime count array */
    pi_arr = (seq_t*)malloc((size_t)(new_limit + 1) * sizeof(seq_t));
    if (!pi_arr) abort();
    pi_arr[0] = 0;
    for (idx_t i = 1; i <= new_limit; ++i)
        pi_arr[i] = pi_arr[i - 1] + (is_prime[i] ? (seq_t)1 : (seq_t)0);

    sieve_limit = new_limit;
}

/* ----- helper: append a new solution to the list -------------------------- */
static void append_solution(seq_t val)
{
    if (sol_len == sol_cap) {
        size_t new_cap = sol_cap == 0 ? 16 : sol_cap * 2;
        seq_t *new_buf = (seq_t*)realloc(solutions, new_cap * sizeof(seq_t));
        if (!new_buf) abort();
        solutions = new_buf;
        sol_cap   = new_cap;
    }
    solutions[sol_len++] = val;
}

/* ----- main sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined for offset 0, return 0 */
        return 0;

    /* generate solutions until we have at least n of them */
    while (sol_len < (size_t)n) {
        /* ensure sieve covers next candidate n = 10*next_k */
        idx_t N = next_k * 10;
        if (N > sieve_limit) {
            idx_t newlimit = sieve_limit == 0 ? 2000000 : sieve_limit * 2;
            if (newlimit < N) newlimit = N * 2;
            extend_sieve(newlimit);
        }

        /* check the equation pi(N) == next_k */
        if (pi_arr[N] == (seq_t)next_k)
            append_solution((seq_t)N);

        ++next_k;
    }

    return solutions[n - 1];     /* 1‑based indexing */
}
