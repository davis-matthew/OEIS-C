/*
 * A105221  a(n) is the sum of n's distinct prime factors below n.
 * https://oeis.org/A105221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of distinct prime factors of n; 0 if n is 1 or prime
 * Range:     u8 n <= 513, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a smallest‑prime‑factor table up to the
 * largest n requested so far.  For small n (≤ ~10⁴) the cost is negligible
 * and the function remains well under the 2‑second time limit per call. */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

/* smallest prime factor array and its allocated size (in elements) */
static uint32_t *spf = NULL;
static size_t spf_cap = 0;

/* Rebuild the SPF table up to *limit* (inclusive).  The routine
 * discards any older table, which keeps the code short and
 * guarantees that spf[i] = smallest prime factor of i for i ≤ limit.
 */
static void
ensure_spf(uint64_t limit)
{
    /* if the current table already covers the needed range, nothing to do */
    if (spf_cap > limit)
        return;

    /* allocate a fresh table of the required size */
    free(spf);                       /* discard old data, if any */
    spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    if (!spf)
        exit(EXIT_FAILURE);

    /* classic sieve: assign spf[i] = i for primes, otherwise the smallest
     * divisor discovered so far.
     */
    for (uint64_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                    /* i is prime */
            spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
            }
        }
    }
    spf_cap = limit + 1;
}

/* Compute a(n) as defined in OEIS A105221 */
seq_t
a(idx_t n)
{
    if (n <= 1)                     /* a(1) = 0 by definition */
        return (seq_t)0;

    /* make sure the SPF table covers n */
    ensure_spf((uint64_t)n);

    uint64_t orig = n;              /* keep the original value for the prime check */
    seq_t sum = 0;

    while (n > 1) {
        uint32_t p = spf[n];
        sum += (seq_t)p;
        while (n % p == 0)
            n /= p;
    }

    /* if n was prime, the sum would be equal to n itself; return 0 instead */
    if (spf[orig] == (uint32_t)orig)
        return (seq_t)0;

    return sum;
}
