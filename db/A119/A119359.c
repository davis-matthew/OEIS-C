/*
 * A119359  Central coefficients of number triangle A119326.
 * https://oeis.org/A119359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A119359 (Central coefficients of triangle A119326)
 *
 *  Formula:
 *
 *   for odd n = 2k+1:
 *       a(n) = ( C(2n , n+1)  +  (-1)^k * Catalan(k) ) / 2
 *
 *   for even n = 2k:
 *       a(n) = ( C(2n , n+1)  -  (-1)^(k-1) * C(2k , k) ) / 2
 *
 *   (C(a,b) is the binomial coefficient and Catalan(k) the k‑th Catalan number.)
 */

#include "../utils/binom.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if (n & 1) {
        /* odd n = 2k+1 */
        idx_t k = (n - 1) / 2;
        seq_t bin = binom(2 * n, n + 1);     /* C(2n , n+1) */
        seq_t cat = catalan(k);              /* Catalan(k) */
        seq_t sum;

        if ((k & 1) == 0) {                 /* (-1)^k = +1 */
            sum = bin + cat;
        } else {                            /* (-1)^k = -1 */
            sum = bin - cat;
        }
        return sum / 2;
    } else {
        /* even n = 2k */
        idx_t k = n / 2;
        seq_t bin = binom(2 * n, n + 1);     /* C(2n , n+1) = C(4k , 2k+1) */
        seq_t c2  = binom(2 * k, k);         /* C(2k , k)        */
        seq_t sum;

        if (((k - 1) & 1) == 0) {             /* (-1)^(k-1) = +1 */
            sum = bin - c2;
        } else {                              /* (-1)^(k-1) = -1 */
            sum = bin + c2;
        }
        return sum / 2;
    }
}
