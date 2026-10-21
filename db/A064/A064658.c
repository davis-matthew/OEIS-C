/*
 * A064658  a(n) = ceiling(prime(n) - n*log(n)).
 * https://oeis.org/A064658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(prime(n) - n*log(n))
 * Range:     u8 n <= 283, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* global static state for prime storage */
static seq_t *prime_list = NULL;
static size_t   prime_cnt  = 0;    /* number of primes stored               */
static size_t   prime_cap  = 0;    /* current capacity of prime_list         */
static idx_t    last_checked = 1;  /* largest integer already tested for prime */

/* Ensure that at least n primes are stored in prime_list. */
static void
ensure_n_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n)
        return;  /* already have enough primes */

    /* Grow the prime array if necessary. */
    if (prime_cap < (size_t)(n * 2)) {
        size_t new_cap = (prime_cap == 0) ? (size_t)(n * 2) : prime_cap * 2;
        seq_t *tmp = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();   /* allocation failure */
        prime_list = tmp;
        prime_cap  = new_cap;
    }

    /* Search for new primes starting from last_checked + 1. */
    idx_t candidate = (last_checked < 2) ? 2 : last_checked + 1;
    while (prime_cnt < (size_t)n) {
        bool is_prime = true;
        long double lim_ld = sqrt((long double)candidate);
        idx_t limit = (idx_t)lim_ld;
        for (size_t i = 0;
             i < prime_cnt && (idx_t)prime_list[i] <= limit;
             ++i) {
            if (candidate % (idx_t)prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            prime_list[prime_cnt++] = (seq_t)candidate;
        }
        last_checked = candidate;
        ++candidate;
    }
}

/*
 * Computes the sequence a(n) for the OEIS entry A064658.
 */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* Undefined for n < 1; return 0 */

    ensure_n_primes(n);                     /* guarantee nth prime ready */

    idx_t prime = (idx_t)prime_list[n - 1];  /* nth prime, 1‑based */

    /* Compute ceil(prime - n*log(n)). Use long double for accuracy. */
    long double val = (long double)prime
                    - (long double)n * logl((long double)n);
    long double ceil_val = ceill(val);
    /* Convert the real result to an integer value that fits in seq_t. */
    return (seq_t)(uint64_t)ceil_val;
}
