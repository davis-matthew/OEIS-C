/*
 * A067549  Determinant of n X n matrix whose diagonal are the first n primes
 *          and all other elements are 1's.
 * https://oeis.org/A067549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = determinant of the n×n matrix with primes on the diagonal
 *            * and ones elsewhere. Recurrence: a(1)=2, * a(n)= (p_n-1)*a(n-1)
 *            + ∏_{i=1}^{n-1}(p_i-1), where p_k is the k‑th prime
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; n>=1 in practice */

    uint32_t *primes = NULL;
    size_t cnt = 0;
    sieve_first_n_primes(n, &primes, &cnt);  /* primes[0] == 2, primes[1] == 3, ... */

    /* Initialise with the first prime (p1 = 2). */
    seq_t ans   = (seq_t)primes[0];          /* a(1) = 2 */
    seq_t prod  = (seq_t)(primes[0] - 1);    /* ∏_{i=1}^{1} (p_i-1) = 1 */

    for (idx_t i = 1; i < n; ++i) {
        seq_t pim1 = (seq_t)(primes[i] - 1);   /* p_i - 1 */
        ans = pim1 * ans + prod;               /* recurrence */
        prod *= pim1;                          /* update product for next step */
    }

    free(primes);  /* memory allocated by the utility */
    return ans;
}
