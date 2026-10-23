/*
 * A121359  Greatest prime factor of pyramidal number A000292(n).
 * https://oeis.org/A121359
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006530(A000292(n)) (greatest prime factor of the
 *            tetrahedral number n*(n+1)*(n+2)/6)
 * Range:     u8 n <= 10, u16 n <= 72, u32 n <= 3142, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* below the offset: value is irrelevant */

    /* Begin with the three consecutive integers */
    seq_t x = (seq_t)n;
    seq_t y = (seq_t)n + 1;
    seq_t z = (seq_t)n + 2;

    /* Reduce division by 2 first to keep intermediate values small */
    if ((x & 1ULL) == 0)
        x /= 2;
    else if ((y & 1ULL) == 0)
        y /= 2;
    else
        z /= 2;

    /* Reduce division by 3 */
    if (x % 3ULL == 0)
        x /= 3;
    else if (y % 3ULL == 0)
        y /= 3;
    else
        z /= 3;

    /* Now the product x*y*z equals n*(n+1)*(n+2)/6 */
    seq_t tetra = x * y * z;

    /* The value of tetra for n ≤ 10 000 comfortably fits in 64 bits,
       so we can safely cast it for use with the provided gpf helper. */
    uint64_t t64 = (uint64_t)tetra;
    uint64_t gpf_val = gpf(t64);

    return (seq_t)gpf_val;
}
