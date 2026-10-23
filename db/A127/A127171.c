/*
 * A127171  Difference between largest prime factors of two successive
 *          3-almost primes.
 * https://oeis.org/A127171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = gpf(3-almost-prime(n+1)) - gpf(3-almost-prime(n))
 * Range:     i8 n <= 82, i16 n <= 82, i32 n <= 82, i64 n <= 82, i128 n <= 82,
 *            big any n
 * Helpers:   factor_u64, gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where gpf(x) is the greatest prime factor of x and
    3-almost-prime(k) is the k-th integer with exactly 3 prime
    factors (counted with multiplicity). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of the build’s typedefs */
extern seq_t a(idx_t n);

/* Utility functions provided by the build */
#include "../utils/factor_u64.h"     /* void factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*); */
#include "../utils/gpf.h"            /* uint64_t gpf(uint64_t); */       /* or largest_prime_factor */

/* Cached greatest prime factors of 3‑almost‑primes: gpf_arr[i] = gpf(A014612(i+1)) */
static seq_t *gpf_arr = NULL;
static idx_t gpf_cnt = 0;       /* number of elements stored so far   */
static idx_t gpf_cap = 0;       /* capacity of the allocated array    */
static uint64_t next_candidate = 2; /* next integer to test for 3‑almost‑prime */

static void extend_to(idx_t need)
{
    if (gpf_cnt >= need)      /* already enough stored */
        return;

    /* Grow the stored array as needed */
    if (gpf_cap < need) {
        idx_t new_cap = gpf_cap ? gpf_cap * 2 : 64;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(gpf_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failure */
        gpf_arr = tmp;
        gpf_cap = new_cap;
    }

    /* Auxiliary arrays for factorization (small size is sufficient) */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t  pcnt;

    /* Scan integers until we have enough 3‑almost‑primes */
    while (gpf_cnt < need) {
        uint64_t x = next_candidate;

        /* Factorize x */
        factor_u64(x, primes, exps, &pcnt);

        int total_factors = 0;
        uint64_t max_prime = 0;
        for (size_t i = 0; i < pcnt; ++i) {
            total_factors += (int)exps[i];
            if (primes[i] > max_prime) max_prime = primes[i];
        }

        if (total_factors == 3) {
            /* Store the greatest prime factor.  Casting to seq_t is allowed. */
            gpf_arr[gpf_cnt] = (seq_t)max_prime;
            ++gpf_cnt;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)      /* undefined for non‑positive indices */
        return 0;

    /* We need the GPFs of the n‑th and (n+1)-th 3‑almost‑primes. */
    extend_to(n + 1);

    idx_t idx1 = n - 1;   /* index of A014612(n)   */
    idx_t idx2 = n;       /* index of A014612(n+1) */

    seq_t diff = gpf_arr[idx2] - gpf_arr[idx1];
    return diff;
}
