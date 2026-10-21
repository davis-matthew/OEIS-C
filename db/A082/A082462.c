/*
 * A082462  Let chi(k) = 1 if prime(k+1) - prime(k) = 2, = 0 otherwise;
 *          sequence gives a(n) = Sum_{k <= n} chi(k).
 * https://oeis.org/A082462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} [prime(k+1) - prime(k) == 2] * This counts
 *            the number of lesser twin prime pairs whose smaller prime * is ≤
 *            prime(n). The function generates the first n+1 primes by * trial
 *            division and then counts adjacent differences of 2
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The offset is 1; for n = 0 we simply return 0. */
    if (n == 0) return (seq_t)0U;

    /* We need at least n+1 primes.  Allocate an array with that capacity. */
    size_t needed = (size_t)(n + 1);
    uint64_t *primes = (uint64_t *)malloc(sizeof(uint64_t) * needed);
    if (!primes) return (seq_t)0U;  /* out of memory; unlikely in tests */

    size_t count = 0;          /* number of primes found so far */
    uint64_t cand = 2;         /* candidate prime to test */

    while (count < needed) {
        bool is_prime = true;
        /* Test divisibility by all stored primes up to sqrt(cand). */
        for (size_t i = 0; i < count; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            primes[count++] = cand;
        }
        ++cand;
    }

    /* Count twin‑prime pairs with the smaller prime ≤ prime(n). */
    seq_t twin = 0;
    for (size_t i = 0; i < n; ++i) {   /* i runs from 0 to n-1 inclusive */
        if (primes[i + 1] - primes[i] == 2) {
            ++twin;
        }
    }

    free(primes);
    return twin;
}
