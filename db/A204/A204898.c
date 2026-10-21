/*
 * A204898  Ordered differences of odd primes.
 * https://oeis.org/A204898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_j - p_i, where p_k is the k-th odd prime, j is the
 *            unique integer satisfying (j-1)*(j-2)/2 < n <= j*(j-1)/2, and i
 *            = n - (j-1)*(j-2)/2. In other words, the sequence enumerates all
 *            differences p_j - p_i (1 ≤ i < j) with the outer loop over j and
 *            the inner loop over i from 1 to j-1
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static cache of odd primes (starting from 3).  All primes up to 20 000
   are generated once; this covers the required range for n up to 10 000. */
static uint32_t *odd_primes = NULL;
static size_t odd_count = 0;

static void init_primes(void) {
    if (odd_primes != NULL)
        return;
    const int LIMIT = 20000;
    bool *sieve = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    for (int i = 2; i <= LIMIT; ++i) sieve[i] = true;
    for (int p = 2; p * p <= LIMIT; ++p) {
        if (sieve[p]) {
            for (int k = p * p; k <= LIMIT; k += p)
                sieve[k] = false;
        }
    }
    odd_primes = (uint32_t *)malloc((LIMIT / 2 + 1) * sizeof(uint32_t));
    size_t idx = 0;
    for (int n = 3; n <= LIMIT; n += 2) {
        if (sieve[n])
            odd_primes[idx++] = (uint32_t)n;
    }
    odd_count = idx;
    free(sieve);
}

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;   /* n starts at 1 in the OEIS offset */
    init_primes();

    /* Find j such that j*(j-1)/2 >= n */
    idx_t low = 1, high = 2;
    while (high * (high - 1) / 2 < n)
        high <<= 1;                       /* double until large enough */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (mid * (mid - 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t j = low;

    /* Position within the block of differences for this j */
    idx_t prev = (j - 1) * (j - 2) / 2;
    idx_t pos = n - prev;               /* 1‑based index inside block */
    idx_t i = pos;                      /* i in [1, j-1] */

    /* Safety check – j will always be within our precomputed range for n <= 10000 */
    if (j > odd_count || i > j - 1)
        return (seq_t)0;

    uint32_t pj = odd_primes[j - 1];     /* j‑th odd prime (0‑based index) */
    uint32_t pi = odd_primes[i - 1];     /* i‑th odd prime */

    return (seq_t)(pj - pi);
}
