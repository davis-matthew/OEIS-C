/*
 * A214385  Number of 3 X 3 X 3 triangular 0..n arrays with no element lying
 *          outside the (possibly reversed) range delimited by its sw and se
 *          neighbors, and no element equal to its horizontal neighbors.
 * https://oeis.org/A214385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(4*C(n+5,6)+19*C(n+4,6)-17*C(n+3,6)+4*C(n+2,6))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where C(k,6) = k*(k-1)*(k-2)*(k-3)*(k-4)*(k-5)/720 and C(k,6)=0 for k<6 */

#include <stdint.h>

static seq_t binom6(idx_t k)
{
    if (k < 6) return 0;
    seq_t r = 1;
    /* multiply k*(k-1)*...*(k-5) */
    for (int i = 0; i < 6; ++i)
        r *= (seq_t)(k - i);
    /* divide by 720 = 6! */
    r /= (seq_t)720;
    return r;
}

seq_t a(idx_t n)
{
    /* The formula is valid for all n >= 1 */
    seq_t c0 = binom6(n + 5); /* C(n+5,6)  */
    seq_t c1 = binom6(n + 4); /* C(n+4,6)  */
    seq_t c2 = binom6(n + 3); /* C(n+3,6)  */
    seq_t c3 = binom6(n + 2); /* C(n+2,6)  */

    seq_t sum = 0;
    sum += 4 * c0;
    sum += 19 * c1;
    sum -= 17 * c2;
    sum += 4 * c3;

    return sum * 2;
}
