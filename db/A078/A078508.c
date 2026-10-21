/*
 * A078508  Number of primes between sqrt(n^3) and sqrt((n+1)^3).
 * https://oeis.org/A078508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of primes p with floor(sqrt(n^3)) < p <
 *            floor(sqrt((n+1)^3))
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   build_primes_upto, floor_sqrt, upper_bound_strict
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"
#include "../utils/upper_bound_strict.h"

static uint64_t *primes = NULL;
static size_t prime_cnt = 0;

/* Find first index i such that primes[i] >= val */
static size_t lower_bound(const uint64_t *arr, size_t n, uint64_t val)
{
    size_t l = 0, r = n;
    while (l < r) {
        size_t m = (l + r) >> 1;
        if (arr[m] < val) l = m + 1;
        else r = m;
    }
    return l;          /* first index with arr[i] >= val */
}

seq_t a(idx_t n)
{
    /* Pre‑compute primes up to a safe upper bound if not already done */
    if (!primes) {
        /* For n up to about 10000 the maximum sqrt((n+1)^3) < 1.1e6.
           Use a generous limit of 2,000,000 to stay safe. */
        size_t limit = 2000000;
        build_primes_upto(limit, &primes, &prime_cnt);
    }

    seq_t n_seq = (seq_t)n;
    seq_t n_cubed = n_seq * n_seq * n_seq;
    seq_t upper_cubed = (n_seq + 1) * (n_seq + 1) * (n_seq + 1);

    seq_t L = floor_sqrt(n_cubed);
    seq_t U = floor_sqrt(upper_cubed);

    uint64_t Lval = (uint64_t)L;
    uint64_t Uval = (uint64_t)U;

    /* Count primes p with L < p < U */
    size_t first_ge_U = lower_bound(primes, prime_cnt, Uval);   /* first p >= U */
    size_t first_gt_L = upper_bound_strict(primes, prime_cnt, Lval); /* first p > L */

    seq_t res = (seq_t)(first_ge_U - first_gt_L);
    return res;
}
