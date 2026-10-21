/*
 * A068320  a(n)=number of arithmetic progressions of primes, strictly
 *          increasing with sum n.
 * https://oeis.org/A068320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of strictly increasing arithmetic progressions of
 *            primes whose sum equals n
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* The sequence is defined only for n >= 1. */
    if (n < 1) return (seq_t)0;

    idx_t N = n;  /* We need to sieve primes up to N. */
    bool *isPrime = (bool *)malloc((N + 1) * sizeof(bool));
    if (!isPrime) return (seq_t)0;

    /* Sieve of Eratosthenes up to N. */
    isPrime[0] = false;
    if (N >= 1) isPrime[1] = false;
    for (idx_t i = 2; i <= N; ++i) isPrime[i] = true;

    for (idx_t p = 2; p * p <= N; ++p) {
        if (isPrime[p]) {
            for (idx_t m = p * p; m <= N; m += p) {
                isPrime[m] = false;
            }
        }
    }

    seq_t count = (seq_t)0;

    /* Length 1: the progression consisting of the prime itself. */
    if (isPrime[N]) {
        count = count + (seq_t)1;
    }

    /* Length k >= 2. */
    for (idx_t k = 2; ; ++k) {
        /* Minimal possible sum for this k is 2k + k(k-1)/2 = k(k+3)/2. */
        seq_t min_sum = (seq_t)k * ((seq_t)k + 3) / (seq_t)2;
        if (min_sum > (seq_t)N) break;

        for (idx_t d = 1; ; ++d) {
            /* Compute d * k * (k-1) / 2 */
            seq_t diag = (seq_t)d * (seq_t)k * ((seq_t)k - (seq_t)1) / (seq_t)2;
            if (diag >= (seq_t)N) break;  /* no larger d will work */

            seq_t diff = (seq_t)N - diag;
            if (diff % (seq_t)k != 0) continue;  /* p must be integer */

            seq_t p = diff / (seq_t)k;
            if (p < (seq_t)2) continue;
            if (!isPrime[(idx_t)p]) continue;

            bool ok = true;
            for (idx_t i = 1; i < k; ++i) {
                if (!isPrime[(idx_t)p + (idx_t)i * d]) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                count = count + (seq_t)1;
            }
        }
    }

    free(isPrime);
    return count;
}
