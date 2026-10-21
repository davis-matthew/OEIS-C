/*
 * A055027  Number of inequivalent Gaussian primes of successive norms
 *          (indexed by A055025).
 * https://oeis.org/A055027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) counts inequivalent Gaussian primes of norm corresponding
 *            to the nth integer in the interleaved list {2} ∪ {primes ≡ 1
 *            (mod 4)} ∪ {p² | p prime ≡ 3 (mod 4)}. The returned value is 1
 *            for a prime of the form p² (including 2), otherwise 2
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Rough upper bound for the nth element in the list.
       30 * n is safely larger than required for n ≤ 10000. */
    idx_t limit = (idx_t)(30 * n);
    if (limit < 10) limit = 10;      /* minimum size */

    while (1) {
        size_t max = (size_t)(limit + 1);
        unsigned char *prime = (unsigned char *)malloc(max);
        unsigned char *sq3   = (unsigned char *)malloc(max);
        if (!prime || !sq3) {
            free(prime);
            free(sq3);
            abort();
        }

        /* ---- sieve of Eratosthenes ---- */
        for (size_t i = 0; i < max; ++i) prime[i] = 1;
        prime[0] = prime[1] = 0;
        size_t sq = (size_t)(sqrt((double)limit) + 0.5);
        for (size_t i = 2; i <= sq; ++i) {
            if (prime[i]) {
                for (size_t j = i * i; j <= limit; j += i)
                    prime[j] = 0;
            }
        }

        /* ---- precompute squares of primes ≡ 3 (mod 4) ---- */
        for (size_t i = 0; i < max; ++i) sq3[i] = 0;
        for (size_t p = 3; p * p <= limit; ++p)
            if (prime[p] && (p & 3) == 3)
                sq3[p * p] = 1;

        /* ---- scan the candidate list until the n-th element is found ---- */
        idx_t count = 0;
        int answer = -1;                 /* 1 or 2 */

        for (idx_t m = 2; m <= limit; ++m) {
            int is_candidate = 0;
            if (m == 2)           is_candidate = 1;
            else if (prime[m] && ((m & 3) == 1)) is_candidate = 1;
            else if (sq3[(size_t)m])          is_candidate = 1;

            if (is_candidate) {
                ++count;
                if (count == n) {
                    answer = (m == 2 || sq3[(size_t)m]) ? 1 : 2;
                    break;
                }
            }
        }

        free(prime);
        free(sq3);

        if (answer != -1)
            return (seq_t)answer;

        /* if not found, enlarge the limit and try again */
        limit *= 2;
    }
}
