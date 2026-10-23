/*
 * A038252  Triangle whose (i,j)-th entry is binomial(i,j)*5^(i-j)*10^j.
 * https://oeis.org/A038252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the (i,j)-th entry of the triangle
 *            T(i,j)=C(i,j)*5^(i-j)*10^j * read by rows, where i and j are
 *            0‑based. That is, * a(0)=T(0,0)=1, * a(1)=T(1,0)=5,
 *            a(2)=T(1,1)=10, a(3)=T(2,0)=25, … * We compute the corresponding
 *            (i,j) for a given index n and then evaluate the * entry using
 *            the supplied u...
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial, pow2, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/power5.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Find the row i such that i*(i+1)/2 ≤ n < (i+1)*(i+2)/2. */
    idx_t low  = 0;
    idx_t high = 1;
    /* Expand high until it is large enough. */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
    }
    /* Binary search for the correct row. */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;   /* upper mid */
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    idx_t i = low;
    idx_t prev = i * (i + 1) / 2;   /* entries before row i */
    idx_t j = n - prev;             /* column within row i */

    /* Compute T(i,j) = binomial(i,j) * 5^(i-j) * 10^j = binomial(i,j) * 5^i * 2^j. */
    seq_t val = binomial(i, j);
    val *= power5(i);
    val *= pow2(j);
    return val;
}
