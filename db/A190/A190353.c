/*
 * A190353  Goldbach conjecture: number of decompositions of n into an
 *          unordered sum of two odd primes (if n even) or three odd primes
 *          (if n odd).
 * https://oeis.org/A190353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 376, u16 n <= 9534, u32 n <= 9534, u64 n <= 9534,
 *            u128 n <= 9534, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A190353:
   a(n) = number of unordered decompositions of n into two odd primes
   if n is even, or into three odd primes if n is odd.
   Formula: for even n, count pairs (p,q) of odd primes with p+q=n;
   for odd n, count triples (p,q,r) of odd primes with p ≤ q ≤ r and
   p+q+r=n. */
/* Note: Returned value fits in seq_t for all n ≤ 10 000. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* n is at least 1; for very small n the answer is 0. */
    if (n < 5) { /* first possible decomposition is 6=3+3, 5=3+3+? no. */
        return (seq_t)0;
    }

    /* Convert index to host type for array indexing. */
    size_t N = (size_t)n;               /* safe because n ≤ 10000 in tests */

    /* Allocate sieve for numbers up to N. */
    bool *is_comp = (bool *)malloc((N + 1) * sizeof(bool));
    if (!is_comp) return (seq_t)0;
    for (size_t i = 0; i <= N; ++i) is_comp[i] = false;
    if (N >= 0) is_comp[0] = is_comp[1] = true;
    for (size_t p = 2; p * p <= N; ++p) {
        if (!is_comp[p]) {
            for (size_t j = p * p; j <= N; j += p)
                is_comp[j] = true;
        }
    }

    /* Collect odd primes up to N (exclude 2). */
    size_t capacity = N / 2 + 1;
    size_t *primes = (size_t *)malloc(capacity * sizeof(size_t));
    if (!primes) { free(is_comp); return (seq_t)0; }
    size_t prime_cnt = 0;
    for (size_t x = 3; x <= N; x += 2) {
        if (!is_comp[x]) primes[prime_cnt++] = x;
    }

    seq_t result = 0;

    if ((N & 1) == 0) { /* even n: unordered pairs of odd primes */
        size_t half = N / 2;
        for (size_t i = 0; i < prime_cnt; ++i) {
            size_t p = primes[i];
            if (p > half) break;
            size_t q = N - p;
            if (q < p) continue;        /* ensure p ≤ q */
            if (q <= N && !is_comp[q] && (q & 1U))
                ++result;
        }
    } else { /* odd n: unordered triples of odd primes */
        for (size_t i = 0; i < prime_cnt; ++i) {
            size_t p = primes[i];
            if (3 * p > N) break;      /* minimal sum with smallest r is 3p */
            for (size_t j = i; j < prime_cnt; ++j) {
                size_t q = primes[j];
                if (p + q > N) break;   /* since r = N - p - q must be >= q */
                size_t r = N - p - q;
                if (r < q) continue;    /* enforce p ≤ q ≤ r */
                if (r <= N && !is_comp[r] && (r & 1U))
                    ++result;
            }
        }
    }

    free(primes);
    free(is_comp);
    return result;
}
