/*
 * A121709  Numerator of Sum/Product of first n Lucas numbers A000032[n].
 * https://oeis.org/A121709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = Numerator[ Sum[Lucas[k], {k,1,n}] / Product[Lucas[k],
 *            {k,1,n}] ]
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, guard anyway */

    /* First pass: compute the sum of the first n Lucas numbers */
    seq_t sum = 0;
    seq_t lprev = 1;      /* Lucas(1) */
    seq_t lcurr = 3;      /* Lucas(2) */
    seq_t lk;

    for (idx_t i = 1; i <= n; ++i) {
        if (i == 1) {
            lk = lprev;
        } else if (i == 2) {
            lk = lcurr;
        } else {
            lk = lprev + lcurr;
            lprev = lcurr;
            lcurr = lk;
        }
        sum += lk;
    }

    /* Second pass: compute the product modulo the sum */
    seq_t prod_mod = 1 % sum;  /* sum > 0 for all n ≥ 1 */
    lprev = 1;
    lcurr = 3;

    for (idx_t i = 1; i <= n; ++i) {
        if (i == 1) {
            lk = lprev;
        } else if (i == 2) {
            lk = lcurr;
        } else {
            lk = lprev + lcurr;
            lprev = lcurr;
            lcurr = lk;
        }
        /* Use __int128 to avoid overflow while multiplying */
        prod_mod = (__int128)prod_mod * lk % sum;
    }

    /* Reduce the fraction: numerator = sum / gcd(sum, prod_mod) */
    seq_t g = seq_gcd(sum, prod_mod);
    return sum / g;
}
