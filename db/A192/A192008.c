/*
 * A192008  Modified linear phone booth sequence: number of ways to occupy n
 *          phone booths in a row, one by one, each time picking a phone booth
 *          adjacent to the smallest number of previously occupied phone
 *          booths.
 * https://oeis.org/A192008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{v1=0..1, v2=0..1, m,k≥0, 2m+3k = n-1-v1-v2} * (m+k+1)!
 *            · binomial(m+k,m) · 2^k · (k+v1+v2)! · (m+k)!
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 15, u64 n <= 24, u128 n <= 40,
 *            big any n
 * Helpers:   binomial, factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/pow2.h"

/* Greatest‑common‑divisor and other utilities are not needed here. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset starts at 1 */

    seq_t sum = 0;
    for (int v1 = 0; v1 <= 1; ++v1) {
        for (int v2 = 0; v2 <= 1; ++v2) {
            idx_t vsum = (idx_t)v1 + (idx_t)v2;
            if (n <= vsum)          /* n-1-vsum < 0 → no terms */
                continue;
            idx_t rem = n - 1 - vsum;
            idx_t maxk = rem / 3;
            for (idx_t k = 0; k <= maxk; ++k) {
                idx_t rem2 = rem - 3 * k;
                if ((rem2 & 1U) != 0)   /* modulo 2 ≠ 0 → no integer m */
                    continue;
                idx_t m = rem2 >> 1;    /* m = (rem2)/2 */
                idx_t mk = m + k;       /* m + k */

                seq_t term = factorial(mk + 1);           /* (m+k+1)! */
                term = term * binomial(mk, m);           /* · C(m+k,m) */
                term = term * pow2(k);                  /* · 2^k */
                term = term * factorial(k + vsum);      /* · (k+v1+v2)! */
                term = term * factorial(mk);            /* · (m+k)!  */

                sum += term;
            }
        }
    }

    return sum;
}
