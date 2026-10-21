/*
 * A095698  Number of permutations of {1,2,3,...,n} where, for 1 < i <= n, the
 *          i-th number has maximized sum of the i-1 absolute differences from
 *          all previous numbers of the permutation.
 * https://oeis.org/A095698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = 2 * 3^(k-1) for k ≥ 1 a(2k+1) = 2 * 3^k - 2^k for k ≥ 0
 *            with a(1)=1, a(2)=2, … as given in the OEIS entry
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1, but safe guard */
        return 0;

    if ((n & 1) == 0) {          /* even: n = 2 * k, k ≥ 1 */
        idx_t k = n >> 1;        /* k = n/2 */
        /* compute 3^(k-1) */
        seq_t pow3 = 1;
        seq_t base = 3;
        idx_t exp = k - 1;
        while (exp) {
            if (exp & 1) pow3 *= base;
            base *= base;
            exp >>= 1;
        }
        return 2 * pow3;
    } else {                     /* odd: n = 2 * k + 1, k ≥ 0 */
        idx_t k = (n - 1) >> 1;   /* k = (n-1)/2 */
        /* compute 3^k */
        seq_t pow3 = 1;
        seq_t base3 = 3;
        idx_t exp = k;
        while (exp) {
            if (exp & 1) pow3 *= base3;
            base3 *= base3;
            exp >>= 1;
        }
        /* compute 2^k */
        seq_t pow2 = 1;
        seq_t base2 = 2;
        exp = k;
        while (exp) {
            if (exp & 1) pow2 *= base2;
            base2 *= base2;
            exp >>= 1;
        }
        return 2 * pow3 - pow2;
    }
}
