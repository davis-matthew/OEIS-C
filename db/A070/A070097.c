/*
 * A070097  Number of integer triangles with perimeter n and prime side
 *          lengths which are both acute and scalene.
 * https://oeis.org/A070097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{p < q < r : all primes, p + q + r = n, p + q > r
 *            (triangle inequality), p^2 + q^2 > r^2 (acute triangle)}| (Only
 *            scalene triangles with prime sides are counted.)
 * Range:     u8 n <= 34, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* No integer triangle with three distinct prime sides can have perimeter < 10,
       and even for larger perimeters the number of triangles is tiny, so we can
       safely return 0 for very small n without building a sieve. */
    if (n < 10) return (seq_t)0;

    /* Build a simple sieve of Eratosthenes up to n.  is_prime[x] == 1 iff x is prime. */
    size_t limit = (size_t)n;
    char *is_prime = (char *)calloc(limit + 1, 1);
    if (!is_prime) return (seq_t)0;
    if (limit >= 2) is_prime[2] = 1;
    for (size_t i = 3; i <= limit; i += 2) is_prime[i] = 1;  /* odd numbers only */

    /* Mark composites */
    for (size_t i = 3; i * i <= limit; i += 2) {
        if (!is_prime[i]) continue;
        for (size_t j = i * i; j <= limit; j += 2 * i) {
            is_prime[j] = 0;
        }
    }

    /* Gather all primes into an array.  Rough upper bound for number of primes is limit/2. */
    size_t max_primes = limit / 2;
    idx_t *primes = (idx_t *)malloc(max_primes * sizeof(idx_t));
    if (!primes) { free(is_prime); return (seq_t)0; }
    size_t pcount = 0;
    if (limit >= 2) primes[pcount++] = 2;
    for (size_t i = 3; i <= limit; i += 2) {
        if (is_prime[i]) primes[pcount++] = i;
    }

    /* Main enumeration of unordered triples p < q < r. */
    seq_t count = 0;
    for (size_t i = 0; i + 2 < pcount; ++i) {
        seq_t p = primes[i];
        for (size_t j = i + 1; j + 1 < pcount; ++j) {
            seq_t q = primes[j];
            seq_t r = (seq_t)n - p - q;
            /* r must be larger than q and positive.  If r <= q, no subsequent q will work. */
            if (r <= q) break;
            /* Triangle inequality: p + q must be strictly greater than r. */
            if (p + q <= r) continue;
            /* r must be prime and within our sieve. */
            if ((size_t)r > limit || !is_prime[(size_t)r]) continue;
            /* Acute condition: p^2 + q^2 > r^2. */
            seq_t lhs = p * p + q * q;
            seq_t rhs = r * r;
            if (lhs <= rhs) continue;
            ++count;
        }
    }

    free(primes);
    free(is_prime);
    return count;
}
