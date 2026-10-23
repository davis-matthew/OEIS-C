/*
 * A175894  a(n) = sigma(d(n)) where d(n) is n-th divisor of 101010 (n=1..64).
 * https://oeis.org/A175894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   a(n) = sigma(d(n)) where d(n) is the n‑th divisor of 101010
 * Range:     u8 n <= 3, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_cmp.h"

seq_t a(idx_t n)
{
    static seq_t divisors[64];
    static seq_t sigma[64];
    static int initialized = 0;
    if (!initialized) {
        const seq_t primes[6] = {2, 3, 5, 7, 13, 37};
        /* generate all 2^6 = 64 divisors */
        for (idx_t mask = 0; mask < 64; ++mask) {
            seq_t prod = 1;
            for (int i = 0; i < 6; ++i)
                if (mask & (1ULL << i))
                    prod *= primes[i];
            divisors[mask] = prod;
        }
        /* sort the divisors */
        qsort(divisors, 64, sizeof(seq_t), seq_cmp);
        /* compute sigma for each divisor */
        for (idx_t i = 0; i < 64; ++i) {
            seq_t d = divisors[i];
            seq_t s = 1;
            for (int j = 0; j < 6; ++j)
                if ((d % primes[j]) == 0)
                    s *= (1 + primes[j]);
            sigma[i] = s;
        }
        initialized = 1;
    }
    if (n == 0 || n > 64)
        return (seq_t)0;
    return sigma[n - 1];
}
