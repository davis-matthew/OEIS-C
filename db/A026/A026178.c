/*
 * A026178  a(n) is the position of n in A026177.
 * https://oeis.org/A026178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n/2 if n is even and the lowest non‑0 ternary digit of n
 *            equals 1; otherwise a(n) = floor(3n/2). The lowest non‑0 ternary
 *            digit can be determined by repeatedly dividing by 3 while the
 *            remainder is 0
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <assert.h>

seq_t a(idx_t n)
{
    assert(n > 0);                 /* sequence defined for n >= 1 */

    /* Find the lowest non‑0 ternary digit of n (value 1 or 2) */
    idx_t m = n;
    while ((m % 3) == 0)            /* skip trailing zeros in base‑3 */
        m /= 3;
    int ldz = m % 3;                /* 1 or 2 */

    if ((n & 1) == 0 && ldz == 1) { /* n even and LNZ = 1 */
        return (seq_t)(n >> 1);     /* n/2 */
    } else {                        /* floor(3n/2) */
        seq_t res = ((seq_t)(n >> 1)) * 3; /* (n/2)*3  */
        if (n & 1)                 /* add 1 if n is odd */
            res += 1;
        return res;
    }
}
