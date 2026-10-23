/*
 * A156876  Number of primes <= n that are safe primes or Sophie Germain
 *          primes.
 * https://oeis.org/A156876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes <= n that are safe primes or Sophie
 *            Germain primes
 * Range:     u8 n <= 8782, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"    /* provides is_prime(idx_t x) */

/* <idx_t> is an alias for uint64_t.  <seq_t> is the output type.  */
seq_t a(idx_t n)
{
    static seq_t *counts = NULL;   /* counts[i] = a(i), counts[0] unused */
    static idx_t max_n = 0;        /* highest n for which counts is computed */
    static seq_t current_count = 0;

    if (n == 0) return 0;          /* out of offset, safe return */

    if (n <= max_n)                /* already computed */
        return counts[n];

    /* extend the cache to cover [max_n+1 .. n] */
    idx_t new_max = n;
    /* Grow the counts array to size new_max+1 (index 0..new_max) */
    seq_t *new_counts = (seq_t *)realloc(counts, (new_max + 1) * sizeof(seq_t));
    if (!new_counts) abort();      /* memory error */
    counts = new_counts;

    /* ensure counts[0] = 0 and previous counts are already correct */
    if (max_n == 0) {
        counts[0] = 0;
        current_count = 0;
    }

    for (idx_t i = max_n + 1; i <= new_max; ++i) {
        if (!is_prime(i)) {
            counts[i] = current_count;   /* no new qualifying prime */
            continue;
        }
        bool qualifies = false;
        /* safe prime: (i-1)/2 must be prime */
        if ((i & 1u) != 0) { /* odd, all primes >2 are odd */
            idx_t q = (i - 1) >> 1;
            if (is_prime(q))
                qualifies = true;
        }
        /* Sophie Germain prime: 2*i+1 must be prime */
        if (!qualifies) {
            /* avoid overflow of 2*i+1 in 64-bit */
            if (i <= ((UINT64_MAX - 1) >> 1)) {
                idx_t two_ip1 = (i << 1) + 1;
                if (is_prime(two_ip1))
                    qualifies = true;
            }
        }
        if (qualifies)
            ++current_count;
        counts[i] = current_count;
    }

    max_n = new_max;
    return counts[n];
}
