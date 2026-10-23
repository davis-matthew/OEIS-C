/*
 * A167689  Natural numbers with swapped twin primes and adjacent nonprimes.
 * https://oeis.org/A167689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS entry A167689:
    Natural numbers permuted by swapping twin primes with their
    adjacent composites.  Algorithm uses a prime sieve, then for
    each maximal twin‑prime chain [p … q] outputs the composites
    in [p … q] first (ascending), followed by the primes in the
    chain descending.  All other numbers remain in place. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Choose a limit large enough to generate at least n terms. */
    size_t limit = (size_t)n * 2 + 1000;
    if (limit < 100) limit = 100;

    /* Simple sieve of Eratosthenes (bool array). */
    unsigned char *isPrime = (unsigned char*)malloc((limit + 1) * sizeof(unsigned char));
    if (!isPrime) return (seq_t)0;
    for (size_t i = 0; i <= limit; ++i) isPrime[i] = 1;
    isPrime[0] = isPrime[1] = 0;
    for (size_t p = 2; p * p <= limit; ++p)
        if (isPrime[p])
            for (size_t m = p * p; m <= limit; m += p)
                isPrime[m] = 0;

    /* Array to hold the permuted sequence up to `limit` values. */
    seq_t *seq = (seq_t*)malloc(limit * sizeof(seq_t));
    if (!seq) { free(isPrime); return (seq_t)0; }

    size_t out_count = 0;
    uint64_t cur = 1;          /* numeric value being processed */

    while (out_count < n) {
        if (cur > limit) {    /* safety: in case limit was too small */
            /* enlarge limit and recompute sieve */
            limit *= 2;
            unsigned char *newPrime = (unsigned char*)realloc(isPrime, (limit + 1) * sizeof(unsigned char));
            if (!newPrime) break;
            isPrime = newPrime;
            for (size_t i = limit/2 + 1; i <= limit; ++i) isPrime[i] = 1;
            for (size_t p = 2; p * p <= limit; ++p)
                if (isPrime[p])
                    for (size_t m = (p * p > limit/p ? limit : p * p); m <= limit; m += p)
                        isPrime[m] = 0;
            seq = (seq_t*)realloc(seq, limit * sizeof(seq_t));
            if (!seq) break;
        }

        if (!isPrime[cur]) {
            seq[out_count++] = (seq_t)cur;
            ++cur;
        } else {
            /* Prime detected.  Determine if it starts a twin‑prime chain. */
            bool startsChain = true;
            if (cur >= 3 && isPrime[cur - 2]) startsChain = false;     /* already handled by previous chain */
            if (startsChain && cur + 2 <= limit && isPrime[cur + 2]) {
                uint64_t start = cur;
                uint64_t end   = cur;
                /* Extend chain while consecutive primes differ by 2 */
                while (end + 2 <= limit && isPrime[end + 2]) end += 2;

                /* Output composites within [start … end] (ascending order) */
                for (uint64_t m = start; m <= end && out_count < n; ++m)
                    if (!isPrime[m]) seq[out_count++] = (seq_t)m;

                /* Output primes within the chain in descending order */
                for (int64_t m = (int64_t)end; m >= (int64_t)start && out_count < n; m -= 2)
                    if (isPrime[(uint64_t)m]) seq[out_count++] = (seq_t)m;

                cur = end + 1;  /* skip the whole block */
            } else {
                /* Prime not part of a twin‑prime chain – output normally */
                seq[out_count++] = (seq_t)cur;
                ++cur;
            }
        }
    }

    seq_t result = seq[n - 1];
    free(isPrime);
    free(seq);
    return result;
}
