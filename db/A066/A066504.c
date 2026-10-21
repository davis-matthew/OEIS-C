/*
 * A066504  Sum of n/p^k over all maximal prime-power divisors of n.
 * https://oeis.org/A066504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p^e || n} n / p^e
 * Range:     u8 n <= 317, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here p^e is each maximal prime‑power divisor of n.  The function
   uses the existing factor_uint64 routine to factor n. */

#include <stdint.h>
#include "../utils/factor_uint64.h"

seq_t a(idx_t n)
{
    /* n = 1 has no prime divisors */
    if (n <= 1)
        return 0;

    uint64_t val = (uint64_t)n;
    uint64_t fac[20];
    int exp[20];
    int cnt;
    factor_uint64(val, fac, exp, &cnt);

    seq_t sum = 0;
    for (int i = 0; i < cnt; ++i) {
        /* compute p^e */
        uint64_t d = 1;
        for (int j = 0; j < exp[i]; ++j)
            d *= fac[i];

        /* n / p^e fits in a 64‑bit integer */
        seq_t term = (seq_t)(val / d);
        sum += term;
    }
    return sum;
}
