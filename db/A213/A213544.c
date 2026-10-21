/*
 * A213544  Sum of numerators of Farey Sequence of order n.
 * https://oeis.org/A213544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + (1/2) * Σ_{k=2..n} k * φ(k) (Farey numerator sum)
 * Range:     u8 n <= 10, u16 n <= 79, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached totient table */
static idx_t *phi = NULL;
static idx_t max_phi_n = 0;

/* Ensure φ(k) is computed for all 1 <= k <= n */
static void _ensure_phi(idx_t n)
{
    if (phi && n <= max_phi_n)
        return;                      /* already available */

    /* Allocate or reallocate for the new size */
    idx_t *new_phi = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!new_phi)
        return;                       /* allocation failure, leave phi NULL */

    /* Initialise with φ(i) = i */
    for (idx_t i = 0; i <= n; ++i)
        new_phi[i] = i;

    /* Classical Euler totient sieve */
    for (idx_t p = 2; p <= n; ++p)
    {
        if (new_phi[p] == p)          /* p is prime */
        {
            for (idx_t j = p; j <= n; j += p)
                new_phi[j] = (new_phi[j] / p) * (p - 1);
        }
    }

    /* Replace old table */
    free(phi);
    phi = new_phi;
    max_phi_n = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;    /* out of range, but harmless */

    if (n == 1)
        return 1;    /* base case */

    _ensure_phi(n);

    seq_t sum = 0;
    for (idx_t m = 2; m <= n; ++m)
        sum += (seq_t)m * (seq_t)phi[m];

    return 1 + sum / 2;
}
