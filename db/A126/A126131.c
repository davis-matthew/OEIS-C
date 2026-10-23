/*
 * A126131  a(n) = number of divisors of n which equal any d(k) for 1 <= k <=
 *          n, where d(k) is the number of positive divisors of k.
 * https://oeis.org/A126131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that d = d(k) for some 1 ≤
 *            k ≤ n, where d(k) is the number of positive divisors of k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache: divisor counts for indices 1..max_n,
   and a flag array marking which divisor‑count values occur
   among 1..max_n.  The cache is rebuilt whenever a larger n is requested. */
static uint64_t *divcnt = NULL;          /* divisor counts for indices 1..max_n */
static unsigned char *present = NULL;    /* present[d] = 1 if some k ≤ max_n has d(k)=d */
static size_t cached_max = 0;            /* maximum n that has been cached */

/* Recompute divisor counts and present flags up to limit (inclusive) */
static void recompute_divcnt(size_t limit)
{
    free(divcnt);
    free(present);

    divcnt  = (uint64_t *)calloc(limit + 1, sizeof(uint64_t));
    present = (unsigned char *)calloc(limit + 1, sizeof(unsigned char));
    if (!divcnt || !present) exit(1);

    /* Sieve-like computation of divisor counts */
    for (size_t i = 1; i <= limit; ++i) {
        for (size_t j = i; j <= limit; j += i)
            divcnt[j]++;          /* count divisor i of j */
    }

    /* Mark which divisor‑count values appear */
    for (size_t i = 1; i <= limit; ++i) {
        size_t d = divcnt[i];
        if (d <= limit)          /* d(k) ≤ k ≤ limit for all k ≤ limit */
            present[d] = 1;
    }

    cached_max = limit;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* sequence offset starts at 1 */

    size_t nn = (size_t)n;

    /* Ensure cache is up to date */
    if (cached_max < nn)
        recompute_divcnt(nn);

    /* Count divisors of n that are marked in present[] */
    idx_t count = 0;
    for (size_t i = 1; i <= nn / i; ++i) {
        if (nn % i == 0) {
            if (present[i]) count++;

            size_t j = nn / i;
            if (j != i && present[j]) count++;
        }
    }

    return (seq_t)count;
}
