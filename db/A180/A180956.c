/*
 * A180956  Triangle read by rows T(n,k) = denominators of A180955/A180956.
 * https://oeis.org/A180956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(2*m - popcount(m)), where m = r - k. r is the row
 *            index satisfying r(r+1)/2 ≤ n < (r+1)(r+2)/2, k = n - r(r+1)/2.
 *            This is the row‑major flattening of the triangle T(r,k) =
 *            A046161(r‑k) = denom(binomial(2*(r‑k),(r‑k))/4^(r‑k))
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 152, u64 n <= 560,
 *            u128 n <= 1325, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    idx_t start = 0;               /* first index of row r */
    /* Using subtraction to avoid overflow */
    while ((n - start) >= (r + 1)) {
        r++;
        start += r;
    }

    idx_t k = n - start;           /* column within the row */
    idx_t m = r - k;               /* argument for A046161 */

    unsigned int pc = __builtin_popcountll((unsigned long long)m);
    idx_t exp = (idx_t)(2 * m - pc); /* exponent of power of 2 */

    return pow2(exp);              /* 2^exp as seq_t */
}
