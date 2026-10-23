/*
 * A103966  Let p(k) = k-th prime. a(n) = smallest prime p(j) such that number
 *          of solutions to p(k)-p(k-1)=m is n, where 1<k<=j.
 * https://oeis.org/A103966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   ensure_seq_array, ensure_uint32_array, ensure_uint64_array,
 *            generate_next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A103966 – Smallest prime with a given gap‑count
   Formula: a(n) = smallest prime p(j) such that some gap m = p(k)-p(k-1)
   has occurred exactly n times among the gaps up to index j. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ensure_seq_array.h"
#include "../utils/ensure_uint32_array.h"
#include "../utils/ensure_uint64_array.h"
#include "../utils/generate_next_prime.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined, return 0 */

    /* static state */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0, primes_cap = 0;
    static uint32_t *gap_counts = NULL;
    static size_t gap_cap = 0;
    static seq_t *a_vals = NULL;
    static size_t a_vals_cap = 0;
    static size_t a_max_count = 0;

    /* initialize once */
    if (primes == NULL) {
        primes = (uint64_t *)malloc(64 * sizeof(uint64_t));
        primes_cnt = 1;
        primes_cap = 64;
        primes[0] = 2;                     /* first prime */
        ensure_uint32_array(&gap_counts, &gap_cap, 32); /* initial gap array */
        ensure_seq_array(&a_vals, &a_vals_cap, 32);
        a_vals[0] = 0;   /* index 0 unused */
    }

    /* produce primes until we have filled a[n] */
    while (a_max_count < (size_t)n) {
        uint64_t new_prime = generate_next_prime(primes[primes_cnt-1],
                                                 &primes, &primes_cnt, &primes_cap);
        uint64_t prev_prime = primes[primes_cnt-2];   /* primes_cnt >=2 now */
        uint64_t gap = new_prime - prev_prime;

        /* count this gap */
        ensure_uint32_array(&gap_counts, &gap_cap, (size_t)gap);
        uint32_t new_count = ++gap_counts[gap];

        /* ensure a_vals is large enough to index by new_count */
        if (new_count > a_max_count) {
            a_max_count = new_count;
            ensure_seq_array(&a_vals, &a_vals_cap, (size_t)a_max_count);
        }

        /* record first occurrence of this count */
        if (a_vals[new_count] == 0) {
            a_vals[new_count] = (seq_t)new_prime;
        }
    }

    return a_vals[n];
}
