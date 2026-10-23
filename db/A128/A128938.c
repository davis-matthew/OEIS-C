/*
 * A128938  Numbers n such that n(n+1) plus the n-th prime is a prime.
 * https://oeis.org/A128938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 58, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th positive integer m (m ≥ 1) such that
   m*(m+1) + p_m is prime, where p_m is the m‑th prime.
   The function iteratively generates primes and tests each n until
   the requested term is found.  It memoises both the primes and
   previously found sequence values so subsequent calls are very fast. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"   /* declares bool is_prime_big(seq_t) */

static seq_t *seq_sol      = NULL;   /* array of found sequence values  */
static size_t  sol_cap     = 0;      /* current capacity of seq_sol     */
static size_t  sol_len     = 0;      /* number of entries in seq_sol     */
static idx_t   last_checked = 0;     /* largest n that has been examined */

static uint64_t *prime_list   = NULL;   /* primes in uint64_t, 1‑based index */
static size_t   prime_cap     = 0;
static size_t   prime_count   = 0;
static uint64_t prime_candidate = 3;   /* next odd number to test               */

/* Make sure the prime list contains at least 'need' primes. */
static void
ensure_primes_up_to(size_t need)
{
    if (prime_count == 0) {          /* first initialization */
        prime_cap = 8;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list) exit(EXIT_FAILURE);
        prime_list[0] = 2;
        prime_count = 1;
    }
    while (prime_count < need) {
        uint64_t cand = prime_candidate;
        bool isprime = true;

        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) { isprime = false; break; }
        }

        if (isprime) {
            if (prime_count == prime_cap) {
                prime_cap = prime_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                    prime_cap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                prime_list = tmp;
            }
            prime_list[prime_count++] = cand;
        }
        prime_candidate += 2;    /* skip even numbers */
    }
}

/* Append a new sequence term (value m) to the memoised array. */
static void
append_solution(idx_t n)
{
    if (sol_len == sol_cap) {
        sol_cap = sol_cap ? sol_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(seq_sol, sol_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        seq_sol = tmp;
    }
    seq_sol[sol_len++] = (seq_t)n;
}

/* Ensure we have computed at least 'target' terms of the sequence. */
static void
ensure_solution_count(idx_t target)
{
    if (target <= sol_len) return;   /* already have enough */

    for (idx_t n = last_checked + 1; ; ++n) {
        ensure_primes_up_to((size_t)n);           /* need p_n */
        seq_t term = (seq_t)n * ((seq_t)n + 1)
                     + (seq_t)prime_list[n - 1];

        if (is_prime_big(term))
            append_solution(n);

        last_checked = n;
        if (sol_len >= (size_t)target) break;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    ensure_solution_count(n);
    return seq_sol[n - 1];
}
