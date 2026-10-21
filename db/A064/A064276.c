/*
 * A064276  Number of 2 X 2 singular integer matrices with elements from
 *          {0,...,n} up to row and column permutation.
 * https://oeis.org/A064276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A059306(n) + (n+1)*(2*n+3))/4 where A059306(n) = (n+1)^2
 *            + Σ_{m=1..n} φ(m)₂ * ⌊n/m⌋(⌊n/m⌋+1) and φ(m)₂ = 2*φ(m) (plus 1
 *            when m==1)
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the number of 2×2 singular integer matrices with
   entries from {0,…,n} up to row and column permutation. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* phi[i] will hold Euler’s totient φ(i) for 0 ≤ i ≤ n */
    idx_t *phi = (idx_t *)malloc((size_t)(n + 1) * sizeof(idx_t));
    if (!phi)
        return 0; /* memory failure – unlikely in tests */
    phi[0] = 0;
    if (n >= 1) phi[1] = 1;

    /* sieve to compute φ(i) for i = 1..n */
    for (idx_t i = 2; i <= n; ++i)
        phi[i] = i;
    for (idx_t p = 2; p <= n; ++p) {
        if (phi[p] == p) {  /* p is prime */
            for (idx_t j = p; j <= n; j += p)
                phi[j] -= phi[j] / p;
        }
    }

    /* A059306(n) */
    seq_t total = (seq_t)(n + 1) * (seq_t)(n + 1);   /* (0,0) column case */

    for (idx_t m = 1; m <= n; ++m) {
        idx_t tmax = n / m;                /* floor(n/m) */
        seq_t cnt = (seq_t)2 * (seq_t)phi[m];
        if (m == 1)                         /* account for (m,m) when m==1 */
            cnt += (seq_t)1;
        total += (seq_t)tmax * (seq_t)(tmax + 1) * cnt;
    }

    free(phi);

    /* a(n) = (A059306(n) + (n+1)*(2*n+3))/4 */
    seq_t offset = (seq_t)(n + 1) * (seq_t)(2 * (seq_t)n + 3);
    return (total + offset) / 4;
}
