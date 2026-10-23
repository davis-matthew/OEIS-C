/*
 * A177719  Number of line segments connecting exactly 3 points in an n X n
 *          grid of points.
 * https://oeis.org/A177719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * ((n-1)*(n-2) + Σ_{i=2}^{⌊n/2⌋} (n-2i)(n-i) φ(i))
 * Range:     u8 n <= 7, u16 n <= 30, u32 n <= 487, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* φ(i) is Euler's totient function. */

#include <stdlib.h>
#include <stdint.h>

/* Static storage for the φ table. */
static uint64_t *phi_vals = NULL;
static idx_t phi_max = 0;

/* Initialise or extend the φ table up to N (inclusive). */
static void init_phi(idx_t N)
{
    if (phi_vals && phi_max >= N)
        return;                       /* already sufficient */

    /* Allocate or reallocate the array. */
    uint64_t *tmp = (uint64_t *)realloc(phi_vals, (N + 1) * sizeof(uint64_t));
    if (!tmp)
        exit(1);                      /* abort if allocation fails */
    phi_vals = tmp;

    /* Initialise φ values. */
    for (idx_t i = 0; i <= N; ++i)
        phi_vals[i] = i;               /* φ(i) = i initially */
    if (N >= 0)                        /* φ(0) is unused but keep zero */
        phi_vals[0] = 0;
    if (N >= 1)
        phi_vals[1] = 1;

    /* Sieve to compute φ for each integer up to N. */
    for (idx_t i = 2; i <= N; ++i) {
        if (phi_vals[i] == i) {          /* i is prime */
            for (idx_t j = i; j <= N; j += i)
                phi_vals[j] -= phi_vals[j] / i;
        }
    }
    phi_max = N;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;                         /* a(1) = a(2) = 0 */

    idx_t limit = n / 2;                  /* floor(n/2) */
    init_phi(limit);                      /* ensure φ values are ready */

    /* Base term: (n-1)*(n-2) */
    seq_t total = (seq_t)(n - 1) * (seq_t)(n - 2);

    /* Sum over i = 2 .. limit. */
    for (idx_t i = 2; i <= limit; ++i) {
        seq_t term = (seq_t)(n - 2 * i) * (seq_t)(n - i)
                     * (seq_t)phi_vals[i];
        total += term;
    }

    return total * 4;                     /* multiply by 4 as per formula */
}
