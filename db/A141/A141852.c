/*
 * A141852  Primes congruent to 5 mod 11.
 * https://oeis.org/A141852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime ≡ 5 (mod 11) – computed by a * sieve and
 *            linear search
 * Range:     u8 n <= 4, u16 n <= 650, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* behaviour below offset is undefined */

    /* Rough upper bound: n * 11 * 15 + 100 is safe for n up to 10 000
     * (≈ 165 000 + 100 for n=10000).  The actual nth prime of this
     * residue class is ~11 n log n, so the bound will always be
     * sufficient and grows only modestly. */
    idx_t limit = n * (idx_t)165 + 100;

    while (1) {
        idx_t *primes = NULL;
        size_t  pcount = 0;

        /* primes array contains all primes ≤ limit, allocated by
         * build_primes. */
        build_primes(limit, &primes, &pcount);

        idx_t found = 0;
        seq_t result = (seq_t)0;

        for (size_t i = 0; i < pcount; ++i) {
            if ((primes[i] % 11ULL) == 5ULL) {
                ++found;
                if (found == n) {
                    result = (seq_t)primes[i];
                    break;
                }
            }
        }

        free(primes);   /* memory allocated inside build_primes */

        if (found >= n) return result;

        /* Not enough primes found: increase the sieve range and try again. */
        limit = limit * 2;
    }
}
