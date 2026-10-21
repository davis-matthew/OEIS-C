/*
 * A072047  Number of prime factors of the squarefree numbers:
 *          omega(A005117(n)).
 * https://oeis.org/A072047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = number of distinct prime factors (omega) of the n-th
 *            squarefree number A005117(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* sequence is defined for n >= 1 */

    /* ------------------------------------------------------------
       Determine a bound that is guaranteed to contain the n-th
       squarefree number.  For x > 0 the count of squarefree numbers
       ≤ x is about 0.6079 x, hence x ≈ 1.645 n.  Using 2 n as a
       safe over‑estimate works fine for n ≤ 10000 and keeps the
       bound small.
     ------------------------------------------------------------ */
    idx_t bound = n * 2 + 100;
    if (bound < 100)
        bound = 100;

    /* ------------------------------------------------------------
       Smallest prime factor array: spf[i] returns the least prime
       dividing i.  A value of 0 indicates that the factor has not
       yet been computed.
     ------------------------------------------------------------ */
    uint32_t *spf = (uint32_t *)calloc((size_t)(bound + 1), sizeof(uint32_t));
    if (!spf)
        return (seq_t)0;   /* allocation failure – return 0 */

    for (idx_t i = 2; i <= bound; ++i) {
        if (!spf[i]) {                     /* i is prime */
            spf[i] = (uint32_t)i;
            if (i * (idx_t)i <= bound) {
                for (idx_t j = i * i; j <= bound; j += i)
                    if (!spf[j]) spf[j] = (uint32_t)i;
            }
        }
    }
    /* every remaining number is prime itself */
    for (idx_t i = 2; i <= bound; ++i)
        if (!spf[i]) spf[i] = (uint32_t)i;

    /* ------------------------------------------------------------
       Compute omega (number of distinct prime factors) and a flag
       indicating whether i is squarefree.
     ------------------------------------------------------------ */
    seq_t *omega      = (seq_t *)calloc((size_t)(bound + 1), sizeof(seq_t));
    uint8_t *sqfree   = (uint8_t *)calloc((size_t)(bound + 1), sizeof(uint8_t));
    if (!omega || !sqfree) {
        free(spf); free(omega); free(sqfree);
        return (seq_t)0;
    }

    sqfree[1] = 1;            /* 1 is squarefree */
    omega[1]  = 0;

    for (idx_t i = 2; i <= bound; ++i) {
        seq_t  count = 0;
        bool   is_sqfree = true;
        uint64_t x = i;

        while (x > 1) {
            uint32_t p = spf[x];          /* a prime factor of x   */
            count++;                      /* it is a new distinct prime */

            x /= p;                       /* remove at least one p  */
            if (x % p == 0)               /* a second occurrence => not squarefree */
                is_sqfree = false;

            while (x % p == 0)            /* strip all copies of p   */
                x /= p;
        }

        omega[i]     = count;
        sqfree[i] = is_sqfree ? 1 : 0;
    }

    /* ------------------------------------------------------------
       Find the n-th squarefree number and return its omega value.
     ------------------------------------------------------------ */
    idx_t found = 0;
    for (idx_t i = 1; i <= bound && found < n; ++i) {
        if (sqfree[i]) {
            ++found;
            if (found == n) {
                seq_t result = omega[i];
                free(spf); free(omega); free(sqfree);
                return result;
            }
        }
    }

    /* The bound was not large enough – unlikely, but handle it. */
    free(spf); free(omega); free(sqfree);
    return (seq_t)0;
}
