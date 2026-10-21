/*
 * A224714  The number of unordered partitions {a,b} of the odd numbers 2n-1
 *          such that one of a and b is composite and the other is prime.
 * https://oeis.org/A224714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 812, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A224714: number of unordered {a,b} partitions of the odd
   number 2n-1 where one part is composite and the other is prime.
   The function performs a simple sieve up to 2n-1 and counts primes p
   such that (2n-1)-p is composite.  No double‑counting occurs because
   each valid partition has a unique prime component. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* offset is 1 */
    /* odd number to partition */
    idx_t m = 2 * n - 1;

    /* Build sieve up to m */
    size_t limit = (size_t)m;
    bool *sieve = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!sieve) return (seq_t)0;           /* allocation failure */

    for (size_t i = 0; i <= limit; ++i) sieve[i] = true;
    if (limit >= 0) sieve[0] = false;
    if (limit >= 1) sieve[1] = false;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = false;
        }
    }

    /* Collect primes up to m */
    size_t estimate = limit / 2 + 1;        /* rough upper bound */
    uint64_t *primes = (uint64_t *)malloc(estimate * sizeof(uint64_t));
    if (!primes) { free(sieve); return (seq_t)0; }

    size_t prime_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (sieve[i]) {
            if (prime_cnt == estimate) {
                estimate *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, estimate * sizeof(uint64_t));
                if (!tmp) { free(primes); free(sieve); return (seq_t)0; }
                primes = tmp;
            }
            primes[prime_cnt++] = (uint64_t)i;
        }
    }

    /* Count qualifying unordered pairs {prime, composite} */
    seq_t count = (seq_t)0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t q = (uint64_t)m - p;
        if (q < 2) continue;             /* composite part must be >= 2 */
        if (!sieve[(size_t)q])           /* q composite */
            ++count;
    }

    free(primes);
    free(sieve);
    return count;
}
