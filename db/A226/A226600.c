/*
 * A226600  Number of ordered triples (i,j,k) with i*j*k <= n and i,j,k >= 0.
 * https://oeis.org/A226600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n*n + 3*n + 1 + Σ_{p=1..n} τ(p) * ⌊n/p⌋ where τ(p) is
 *            the number of positive divisors of p. The function uses a simple
 *            divisor sieve to compute τ(p) for all p up to n and then
 *            performs the summation in O(n) time and O(n) memory
 * Range:     u8 n <= 8, u16 n <= 144, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *tau_cache = NULL;   /* divisor counts up to current size   */
static idx_t   tau_size  = 0;        /* maximum index for which tau is cached */

/* Rebuild the divisor count cache for all p = 1 .. n (inclusive). */
static void rebuild_tau(idx_t n)
{
    /* If needed, grow the cache. */
    if (n > tau_size) {
        size_t sz = (size_t)(n + 1);
        uint32_t *tmp = (uint32_t *)realloc(tau_cache, sz * sizeof(uint32_t));
        if (!tmp) abort();          /* Out of memory */
        tau_cache = tmp;
        tau_size = n;
    }

    /* Zero‑fill the cache up to n. */
    memset(tau_cache, 0, (size_t)(n + 1) * sizeof(uint32_t));

    /* Sieve: for each divisor d, increment all multiples. */
    for (idx_t d = 1; d <= n; ++d) {
        for (idx_t m = d; m <= n; m += d) {
            ++tau_cache[m];
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Ensure divisor counts are available up to n. */
    if (n > tau_size)
        rebuild_tau(n);

    /* Sum Σ_{p=1..n} τ(p) * ⌊n/p⌋. */
    seq_t S = 0;
    for (idx_t p = 1; p <= n; ++p) {
        seq_t divs = (seq_t)tau_cache[p];
        seq_t qp = n / p;          /* integer division */
        S += divs * qp;
    }

    /* Compute the remaining terms: 3*n*n + 3*n + 1 + S. */
    seq_t nn = n;                  /* cast n to seq_t first */
    seq_t result = 3 * nn * nn;    /* 3*n^2 */
    result += 3 * nn;              /* +3*n */
    result += 1;                   /* +1 */
    result += S;                   /* +S */

    return result;
}
