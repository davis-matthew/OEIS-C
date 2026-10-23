/*
 * A152848  Triangle read by rows, A007318 rows repeated ten times .
 * https://oeis.org/A152848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = binomial(r, i) where r is the row index such that *
 *            5*r*(r+1) ≤ n < 5*(r+1)*(r+2) and i = (n – 5*r*(r+1)) % (r+1)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                     /* for UINT64_MAX */
#include "../utils/binomial.h"           /* seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    /* Find the minimal r with 5*(r+1)*(r+2) > n.  At loop exit,
     * 5*r*(r+1) ≤ n. */
    idx_t r = 0;
    while (1) {
        idx_t a1 = r + 1;
        idx_t a2 = r + 2;

        /* If a1*a2 would overflow, then the product is larger than any
         * possible n/5, so the current r is the desired one. */
        if (a1 > UINT64_MAX / a2)
            break;

        idx_t prod = a1 * a2;          /* (r+1)*(r+2) */
        if (prod > n / 5)               /* 5*prod > n  */
            break;

        ++r;
    }

    /* Position inside the block of row r. */
    idx_t offset = n - 5 * r * (r + 1); /* 5*r*(r+1) fits in 64‑bit */
    idx_t i = offset % (r + 1);

    return binomial(r, i);
}
