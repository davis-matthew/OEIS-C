/*
 * A183761  Number of 2 X 2 nonsingular 0..n matrices with rows in increasing
 *          order.
 * https://oeis.org/A183761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+1)^4 - S(n))/2 where S(n) counts singular 2×2
 *            matrices with entries in {0,…,n}: S(n) = (n+1)^2 + 2·n·(n+1) +
 *            Σ_{m=1..n} C(m)·(⌊n/m⌋+1)·⌊n/m⌋ with C(1)=1 and C(m)=2·φ(m) for
 *            m>1 (φ = Euler’s totient)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t *phi = NULL;
static idx_t phi_size = 0;

/* compute Euler's totient up to limit */
static void compute_phi(idx_t limit)
{
    if (limit <= phi_size) return;
    free(phi);
    phi_size = limit;
    phi = (idx_t*)malloc((phi_size + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= phi_size; ++i) phi[i] = i;
    if (phi_size >= 1) phi[1] = 1;
    for (idx_t p = 2; p <= phi_size; ++p)
    {
        if (phi[p] == p) /* p is prime */
        {
            for (idx_t k = p; k <= phi_size; k += p)
                phi[k] -= phi[k] / p;
        }
    }
}

seq_t a(idx_t n)
{
    /* precompute totients up to n */
    compute_phi(n);

    /* total number of matrices (n+1)^4 */
    seq_t t = (seq_t)(n + 1);
    seq_t t2 = t * t;          /* (n+1)^2 */
    seq_t t4 = t2 * t2;        /* (n+1)^4 */

    /* count singular matrices */
    seq_t sumS = 0;

    /* part 1: (n+1)^2 */
    sumS += (seq_t)(n + 1) * (seq_t)(n + 1);

    /* part 2: 2·n·(n+1) */
    sumS += (seq_t)2 * (seq_t)n * (seq_t)(n + 1);

    /* part 3: Σ_{m=1..n} C(m)·(⌊n/m⌋+1)·⌊n/m⌋ */
    for (idx_t m = 1; m <= n; ++m)
    {
        idx_t fm = n / m;
        seq_t cm = (m == 1) ? (seq_t)1 : (seq_t)2 * (seq_t)phi[m];
        seq_t term = cm * ((seq_t)fm + 1) * (seq_t)fm;
        sumS += term;
    }

    /* nonsingular matrices */
    seq_t nonsing = t4 - sumS;

    /* a(n) = nonsing / 2 */
    return nonsing / 2;
}
