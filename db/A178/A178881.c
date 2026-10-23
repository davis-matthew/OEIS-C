/*
 * A178881  Sum of all pairs of greatest common divisors for (i,j) where 1 <=
 *          i < j <= n.
 * https://oeis.org/A178881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∑_{k=1..n} φ(k)·⌊n/k⌋² – n·(n+1)/2)/2
 * Range:     u8 n <= 13, u16 n <= 207, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* 0‑based array holding φ(k) for k ≥ 0 */
static idx_t *phi = NULL;
static idx_t phi_size = 0;

/* Ensure φ array is computed up to n (inclusive). */
static void ensure_phi(idx_t n)
{
    if (phi_size <= n) {
        /* Allocate new array (including index 0) */
        idx_t *new_phi = (idx_t*)malloc((n + 1) * sizeof(idx_t));
        if (!new_phi) return;          /* out‑of‑memory: caller will fail */

        /* Initialise φ values */
        for (idx_t i = 0; i <= n; ++i) new_phi[i] = i;
        /* Euler's totient sieve */
        for (idx_t i = 2; i <= n; ++i) {
            if (new_phi[i] == i) { /* i is prime */
                for (idx_t j = i; j <= n; j += i) {
                    new_phi[j] -= new_phi[j] / i;
                }
            }
        }
        /* φ(1) should be 1; φ(0) stays 0 */
        if (n >= 1) new_phi[1] = 1;

        /* Replace old array */
        free(phi);
        phi = new_phi;
        phi_size = n + 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, safe guard */

    ensure_phi(n);                /* φ values up to n are ready */

    seq_t total = (seq_t)0;
    seq_t nn = (seq_t)n;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t t = n / k;                     /* floor(n/k) */
        seq_t term = (seq_t)phi[k] * (seq_t)t * (seq_t)t; /* φ(k)·t² */
        total += term;
    }

    seq_t half_sum = nn * (nn + 1) / 2;   /* n·(n+1)/2 */
    total -= half_sum;                    /* subtract */
    total /= 2;                           /* divide by 2 */

    return total;
}
