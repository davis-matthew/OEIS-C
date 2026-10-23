/*
 * A185382  Sum_{j=1..n-1} P(n)-P(j), where P(j) = A065091(j) is the j-th odd
 *          prime.
 * https://oeis.org/A185382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n-1} (P(n) - P(j)) where P(j) is the j‑th odd
 *            prime (P(j)=prime(j+1) in standard 1‑based prime indexing). The
 *            function uses a sieve to generate the first n+1 primes on the
 *            first call, then reuses the cached data for subsequent calls.
 *            Complexity: O(n) for the first computation, th...
 * Range:     u8 n <= 11, u16 n <= 144, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *primes      = NULL;   /* primes[0] = 2, primes[1] = 3, ... */
static size_t   primes_cnt   = 0;      /* number of primes stored */
static seq_t  *prefix_sums   = NULL;   /* prefix_sums[i] = sum_{k=1..i} primes[k] */
static size_t   prefix_len   = 0;      /* length of prefix_sums (==primes_cnt) */

/* ensure that at least the first n+1 primes are available */
static void ensure_primes(idx_t n)
{
    size_t needed = (size_t)n + 1;          /* we need primes[0..n] */
    if (primes && primes_cnt > needed)
        return;                             /* already have enough */

    /* allocate new prime array */
    uint32_t *newp   = NULL;
    size_t    newcnt = 0;
    sieve_first_n_primes((idx_t)needed, &newp, &newcnt);

    /* free old memory */
    if (primes)   free(primes);
    if (prefix_sums) free(prefix_sums);

    primes     = newp;
    primes_cnt = newcnt;

    /* build prefix sums of odd primes (skipping primes[0] == 2) */
    prefix_sums = (seq_t*)calloc(newcnt, sizeof(seq_t));
    prefix_len   = newcnt;
    seq_t sum = 0;
    for (size_t i = 1; i < newcnt; ++i) {
        sum += (seq_t)primes[i];
        prefix_sums[i] = sum;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                        /* undefined for n<1, return 0 safely */

    ensure_primes(n);                     /* primes[0..n] ready */

    seq_t p_n = (seq_t)primes[n];         /* P(n) = primes[n] (odd prime) */
    seq_t sum_before = (n > 1) ? prefix_sums[n-1] : 0; /* sum_{j=1}^{n-1} P(j) */

    return (seq_t)(n-1) * p_n - sum_before;
}
