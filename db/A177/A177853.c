/*
 * A177853  Partial sums of A018805.
 * https://oeis.org/A177853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * sum_{k=1..n} φ(k) * (n - k + 1) - n
 * Range:     u8 n <= 9, u16 n <= 67, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 2 * ((n+1) * S(n) - T(n)) - n, where S(n)=∑φ(k) and T(n)=∑k*φ(k) */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* not part of the OEIS offset */

    /* Allocate temporary array for φ(1..n) */
    size_t sz = (size_t)n + 1;
    uint64_t *phi = (uint64_t *)malloc(sz * sizeof(uint64_t));
    if (!phi)
        abort();                        /* memory error */

    /* Initialize φ(i)=i and sieve totients */
    for (idx_t i = 0; i <= n; ++i)
        phi[i] = i;

    for (idx_t p = 2; p <= n; ++p) {
        if (phi[p] == (uint64_t)p) {        /* p is prime */
            for (idx_t k = p; k <= n; k += p)
                phi[k] -= phi[k] / p;
        }
    }

    /* Compute prefix sums S and T */
    seq_t S = 0;
    seq_t T = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t pk = phi[k];
        S += pk;
        T += (seq_t)k * pk;
    }

    seq_t n_seq    = (seq_t)n;
    seq_t n_plus   = n_seq + 1;
    seq_t result   = 2 * ((n_plus * S) - T) - n_seq;

    free(phi);
    return result;
}
