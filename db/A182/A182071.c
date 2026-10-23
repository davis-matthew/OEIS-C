/*
 * A182071  Number of primes in the half-open interval [n*sqrt((n-1)/2),
 *          (n+1)*sqrt(n/2)).
 * https://oeis.org/A182071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p such that * p ∈ [ n * sqrt((n-1)/2) ,
 *            (n+1) * sqrt(n/2) )
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;       /* offset 1, undefined below 1 */

    /* ---------- static initialization of pi array ---------- */
    static bool init = false;
    static uint32_t *pi = NULL;
    /* We need all primes up to the maximum right endpoint for n ≤ 10 000.
       (n+1)*sqrt(n/2) ≈ 7.07×10⁵ for n = 10 000, so 1 000 000 is safe. */
    const uint32_t PI_LIMIT = 1000000U;

    if (!init) {
        /* allocate and build prime sieve */
        pi = (uint32_t *)calloc(PI_LIMIT + 1, sizeof(uint32_t));
        if (!pi) return (seq_t)0; /* allocation failed : fallback to 0 */

        bool *not_prime = (bool *)calloc(PI_LIMIT + 1, sizeof(bool));
        if (!not_prime) { free(pi); return (seq_t)0; }

        uint32_t count = 0;
        pi[0] = pi[1] = 0;

        for (uint32_t i = 2; i <= PI_LIMIT; ++i) {
            if (!not_prime[i]) {
                ++count;
                if (i <= PI_LIMIT / i) {
                    for (uint32_t j = i * i; j <= PI_LIMIT; j += i)
                        not_prime[j] = true;
                }
            }
            pi[i] = count;
        }

        free(not_prime);
        init = true;
    }

    /* ---------- compute interval bounds ---------- */
    seq_t n_seq = (seq_t)n;

    seq_t left_sqr  = n_seq * n_seq * ((seq_t)(n-1)) / (seq_t)2;
    seq_t right_sqr = (n_seq + (seq_t)1) * (n_seq + (seq_t)1) * n_seq / (seq_t)2;

    seq_t left_floor = floor_sqrt(left_sqr);
    seq_t left_floor_sq = left_floor * left_floor;
    seq_t ceil_left = (left_floor_sq == left_sqr) ? left_floor : left_floor + (seq_t)1;

    seq_t right_floor = floor_sqrt(right_sqr);
    seq_t right_floor_sq = right_floor * right_floor;
    seq_t high = (right_floor_sq == right_sqr) ? (right_floor - (seq_t)1)
                                                : right_floor;

    if (ceil_left > high) return (seq_t)0;

    /* ---------- count primes within [ceil_left, high] ---------- */
    uint32_t high_idx = (uint32_t)high;
    uint32_t low_idx_minus1 = (uint32_t)(ceil_left - (seq_t)1);

    uint32_t prime_count = 0;
    if (ceil_left == (seq_t)0)
        prime_count = pi[high_idx];
    else
        prime_count = pi[high_idx] - pi[low_idx_minus1];

    return (seq_t)prime_count;
}
