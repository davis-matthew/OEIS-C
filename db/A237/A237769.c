/*
 * A237769  Number of primes p < n with pi(n-p) - 1 and pi(n-p) + 1 both
 *          prime, where pi(.) is given by A000720.
 * https://oeis.org/A237769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p < n such that * pi(n-p) - 1 and
 *            pi(n-p) + 1 are both prime * (where pi(.) is the prime-counting
 *            function A000720)
 * Range:     u8 n <= 2255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, compute_pi, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper utilities from the utils library */
#include "../utils/is_prime.h"          /* bool is_prime(idx_t x); */
#include "../utils/compute_pi.h"       /* seq_t compute_pi(idx_t n); */
#include "../utils/build_primes_upto.h"/* void build_primes_upto(idx_t limit,
                                      uint64_t **primes_out, size_t *cnt_out); */

/* Global state for the prime list */
static uint64_t *global_primes = NULL;   /* array of primes up to prime_limit */
static size_t   global_prime_cnt = 0;    /* number of primes stored */
static idx_t    prime_limit = 0;         /* highest n for which primes are known */

/* Ensure that the list of primes contains all primes < n */
static void
ensure_primes_upto(idx_t n)
{
    if (n <= prime_limit)
        return;                           /* already prepared */

    /* Rebuild the prime list up to n */
    if (global_primes != NULL) {
        free(global_primes);
        global_primes = NULL;
    }
    build_primes_upto(n, &global_primes, &global_prime_cnt);
    prime_limit = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;                   /* a(1) = 0 */

    /* Make sure we have all primes < n */
    ensure_primes_upto(n);

    seq_t count = 0;

    /* Iterate over primes p < n */
    for (size_t idx = 0; idx < global_prime_cnt; ++idx) {
        uint64_t p = global_primes[idx];
        if (p >= n)
            break;

        idx_t m = n - p;                   /* n - p is the argument to pi() */
        seq_t pi_val = compute_pi(m);       /* pi(m) */

        /* Need pi_val - 1 and pi_val + 1 to be prime; skip if pi_val < 2 */
        if (pi_val < 2)
            continue;

        idx_t pi_minus = (idx_t)(pi_val - 1);
        idx_t pi_plus  = (idx_t)(pi_val + 1);

        if (is_prime(pi_minus) && is_prime(pi_plus))
            ++count;
    }

    return count;
}
