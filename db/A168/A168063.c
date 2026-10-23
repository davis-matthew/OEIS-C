/*
 * A168063  Numbers k such that exactly two of {k-2, k-1, k+1, k+2} are
 *          primes.
 * https://oeis.org/A168063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168063: numbers k such that exactly two of {k-2, k-1, k+1, k+2}
 * are primes.  We compute the n-th element by scanning k from 1 upward
 * until we find n elements satisfying the condition.  No caching is
 * required – each call recomputes from the start and returns the correct
 * value for any n ≥ 1. */
#include <stdbool.h>
#include "../utils/is_prime_small.h"

static inline int count_prime_neighbors(uint64_t k)
{
    int c = 0;
    if (k >= 3 && is_prime_small((seq_t)(k - 2))) c++;
    if (k >= 2 && is_prime_small((seq_t)(k - 1))) c++;
    if (is_prime_small((seq_t)(k + 1)))          c++;
    if (is_prime_small((seq_t)(k + 2)))          c++;
    return c;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    uint64_t k = 1;
    while (true) {
        if (count_prime_neighbors(k) == 2) {
            found++;
            if (found == n) return (seq_t)k;
        }
        k++;
    }
}
