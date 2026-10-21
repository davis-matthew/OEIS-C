/*
 * A238268  The number of unordered ways that n can be written as the sum of
 *          two numbers of the form p or 2p, where p is prime.
 * https://oeis.org/A238268
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered representations of n as the sum of
 *            two numbers of the form p or 2p, where p is prime
 * Range:     u8 n <= 4199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 4; return 0 for smaller indices. */
    if (n < 4)
        return (seq_t)0;

    /* Generate all primes ≤ n. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    /* Allocate room for the values p and 2p (each ≤ n). */
    size_t vals_cap = prime_cnt * 2;                      /* worst‑case size */
    uint64_t *vals = (uint64_t *)malloc(vals_cap * sizeof(uint64_t));
    if (!vals) {
        free(primes);
        return (seq_t)0;
    }

    /* Boolean marker for O(1) membership test. */
    uint8_t *mark = (uint8_t *)calloc((size_t)n + 1, sizeof(uint8_t));
    if (!mark) {
        free(primes);
        free(vals);
        return (seq_t)0;
    }

    /* Build the list of values and mark them. */
    size_t vals_cnt = 0;
    for (size_t i = 0; i < prime_cnt && vals_cnt < vals_cap; ++i) {
        uint64_t p = primes[i];
        if (p <= n) {
            mark[p] = 1;
            vals[vals_cnt++] = p;
        }
        uint64_t d = p << 1;    /* 2p */
        if (d <= n) {
            mark[d] = 1;
            vals[vals_cnt++] = d;
        }
    }

    /* Count unordered pairs (x, y) with x + y = n. */
    seq_t count = 0;
    for (size_t i = 0; i < vals_cnt; ++i) {
        uint64_t x = vals[i];
        uint64_t y = n - x;
        if (y >= x && y <= n && mark[y])
            ++count;
    }

    free(primes);
    free(vals);
    free(mark);
    return count;
}
