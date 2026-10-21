/*
 * A099777  Number of divisors of 2n.
 * https://oeis.org/A099777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9355, u16 n <= 9355, u32 n <= 9355, u64 n <= 9355,
 *            u128 n <= 9355, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A099777: Number of divisors of 2*n.  a(n) = d(2n),
   where d(x) is the number of positive divisors of x. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_uint64.h"  // provides factor_uint64

seq_t a(idx_t n)
{
    /* n is 1‑based; for n==0 return 0 (out of range). */
    if (n == 0) return 0;

    uint64_t n64 = (uint64_t)n;

    /* Count the power of two in n. */
    int e = 0;
    uint64_t odd = n64;
    while ((odd & 1ULL) == 0) {
        odd >>= 1;
        e++;
    }

    /* Factor the odd part; determine product of (exponent+1). */
    seq_t prod = 1;
    if (odd > 1) {
        uint64_t fac[20];
        int exp[20];
        int cnt = 0;
        factor_uint64(odd, fac, exp, &cnt);   /* factor_uint64 is pre‑included */
        for (int i = 0; i < cnt; i++) {
            prod *= (seq_t)(exp[i] + 1);
        }
    }

    /* d(2n) = (e+2) * prod.  For e==0 (n odd), prod = d(n). */
    seq_t result = ((seq_t)(e + 2)) * prod;
    return result;
}
