/*
 * A117898  Number triangle 2^abs(L(C(n,2)/3) - L(C(k,2)/3))*[k<=n] where
 *          L(j/p) is the Legendre symbol of j and p.
 * https://oeis.org/A117898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 2^abs(L(C(r,2)/3) – L(C(k,2)/3)) where 0 <= k <= r and n
 *            = r*(r+1)/2 + k. L(j/3) is the Legendre symbol modulo 3
 * Range:     u8 n <= 5150, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The triangle is read row‑wise; this function returns the element at
   the linear index n. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((uint64_t)((__int128)(r + 1) * (r + 2) / 2) <= n) {
        ++r;
    }

    /* Start index of this row. */
    idx_t start = (uint64_t)((__int128)r * (r + 1) / 2);
    idx_t k = n - start;   /* Column index within the row. */

    /* C(m,2) mod 3 is 0 iff m % 6 is 0,1,3,4.
       It is 1 iff m % 6 is 2 or 5.  Thus the Legendre symbol L(C(m,2)/3)
       equals 1 when m % 6 is 2 or 5, otherwise 0. */
    bool r_not_div = (r % 6 == 2u || r % 6 == 5u);
    bool k_not_div = (k % 6 == 2u || k % 6 == 5u);

    return (r_not_div ^ k_not_div) ? (seq_t)2 : (seq_t)1;
}
