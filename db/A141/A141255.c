/*
 * A141255  Total number of line segments between points visible to each other
 *          in a square n X n lattice.
 * https://oeis.org/A141255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(n-1)*(2*n-1) + 2*Σ_{i=2}^{n-1} (n-i)*(2*n-i)*phi(i)
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 344, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* with a(1) = 0 */

#include <stdlib.h>

/* Caches Φ(i) values up to the largest n requested so far. */
static seq_t *phi_cache = NULL;
static idx_t phi_cache_len = 0;

/* Recompute Φ(1..limit) from scratch. */
static void ensure_phi(idx_t limit)
{
    if (limit <= phi_cache_len)
        return;

    /* Allocate space for Φ[0..limit] */
    if (phi_cache != NULL)
        free(phi_cache);
    phi_cache = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (phi_cache == NULL)
        exit(1);    /* out of memory (unlikely for n ≤ 10000) */

    /* Initialise Φ[i] = i for 1 ≤ i ≤ limit */
    for (idx_t i = 1; i <= limit; ++i)
        phi_cache[i] = (seq_t)i;

    /* Euler's totient sieve */
    for (idx_t i = 2; i <= limit; ++i) {
        if (phi_cache[i] == (seq_t)i) {            /* i is prime */
            for (idx_t j = i; j <= limit; j += i) {
                phi_cache[j] = phi_cache[j] / i * (i - 1);
            }
        }
    }
    phi_cache_len = limit;
}

/* Compute A141255(a(n)). */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Need φ(i) for 1 ≤ i ≤ n‑1 */
    ensure_phi(n - 1);

    /* Main term: 2*(n-1)*(2n-1) */
    seq_t term = 2 * (seq_t)(n - 1) * ((seq_t)(2 * n) - 1);

    /* Sum over i = 2..n‑1 */
    for (idx_t i = 2; i <= n - 1; ++i) {
        seq_t add =
            (seq_t)(n - i) * ((seq_t)(2 * n) - i) * phi_cache[i];
        term += 2 * add;
    }
    return term;
}
