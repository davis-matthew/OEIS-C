/*
 * A161756  The sum of all numbers from n up to A002808(n)-3.
 * https://oeis.org/A161756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = T(A002808(n)-3) - T(n-1) where T(m) = m*(m+1)/2
 * Range:     u8 n <= 21, u16 n <= 518, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cache of the first composites: comps[0] = 4, comps[1] = 6, ... */
static idx_t *comps = NULL;
static size_t comps_cap = 0;

/* Ensure that comps has at least n composite numbers (1‑based). */
static void ensure_composites(idx_t n)
{
    if (n <= comps_cap) return;

    /* Load enough composites with a simple sieve. A bound of
       2*n + 10 is comfortably larger than the n‑th composite for
       any n that fits in the built idx_t type. */
    idx_t limit = 2 * n + 10;

    unsigned char *is_prime = (unsigned char *)malloc((size_t)(limit + 1));
    if (!is_prime) abort();
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime[j] = 0;
        }
    }

    /* Collect the first n composites. */
    idx_t *new_comps = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!new_comps) abort();
    size_t cnt = 0;
    for (idx_t i = 4; i <= limit && cnt < n; ++i) {
        if (!is_prime[i]) {
            new_comps[cnt++] = i;
        }
    }

    free(is_prime);
    free(comps);
    comps = new_comps;
    comps_cap = cnt;
}

/* Triangular number T(m) = m*(m+1)/2 computed in seq_t. */
static inline seq_t triangular(idx_t m)
{
    return ((seq_t)m * ((seq_t)m + 1)) / 2;
}

/* OEIS sequence A161756. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out‑of‑range guard */

    ensure_composites(n);

    idx_t comp = comps[n - 1];          /* A002808(n)  */
    seq_t part1 = triangular((idx_t)(comp - 3));
    seq_t part2 = triangular(n - 1);
    return part1 - part2;
}
