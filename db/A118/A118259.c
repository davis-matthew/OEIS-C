/*
 * A118259  Numbers of strongly carefree couples (a,b) with a,b <= n.
 * https://oeis.org/A118259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i,j=1..n} mu(i*j)^2
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* --------- 1. Sieve for mobius and squarefree list --------- */
    int   *mu      = (int*)malloc((n + 1) * sizeof(int));
    bool  *is_sq   = (bool*)malloc((n + 1) * sizeof(bool));
    bool  *is_cmp  = (bool*)malloc((n + 1) * sizeof(bool));
    idx_t *primes  = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    if (!mu || !is_sq || !is_cmp || !primes) {
        free(mu); free(is_sq); free(is_cmp); free(primes);
        return 0;
    }

    int   pcnt = 0;
    for (idx_t i = 0; i <= n; ++i) {
        mu[i]   = 0;
        is_sq[i] = false;
        is_cmp[i] = false;
    }
    mu[1] = 1;  /* mu(1) = 1 */
    for (idx_t i = 2; i <= n; ++i) {
        if (!is_cmp[i]) {                 /* i is prime */
            primes[pcnt++] = i;
            mu[i] = -1;
        }
        for (int j = 0; j < pcnt && i * primes[j] <= n; ++j) {
            idx_t x = i * primes[j];
            is_cmp[x] = true;
            if (i % primes[j] == 0) {     /* p^2 divides x => not squarefree */
                mu[x] = 0;
                break;
            } else {
                mu[x] = -mu[i];
            }
        }
    }

    /* Identify squarefree numbers and build list */
    idx_t mcnt = 0;
    for (idx_t i = 1; i <= n; ++i)
        if (mu[i] != 0) {
            is_sq[i] = true;
            ++mcnt;
        }

    idx_t *sq = (idx_t*)malloc(mcnt * sizeof(idx_t));
    if (!sq) {
        free(mu); free(is_sq); free(is_cmp); free(primes);
        return 0;
    }
    idx_t pos = 0;
    for (idx_t i = 1; i <= n; ++i)
        if (is_sq[i]) sq[pos++] = i;

    /* --------- 2. Count ordered coprime squarefree pairs --------- */
    seq_t total = 0;
    for (idx_t ii = 0; ii < mcnt; ++ii) {
        seq_t ai = (seq_t)sq[ii];
        for (idx_t jj = 0; jj < mcnt; ++jj) {
            if (gcd(ai, (seq_t)sq[jj]) == 1)
                ++total;
        }
    }

    /* --------- 3. Clean up and return --------- */
    free(mu); free(is_sq); free(is_cmp); free(primes); free(sq);
    return total;
}
