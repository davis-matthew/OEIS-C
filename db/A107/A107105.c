/*
 * A107105  Triangle, read by rows, where T(n,k) = C(n,k)*(C(n,k) + 1)/2,
 *          n>=k>=0.
 * https://oeis.org/A107105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   A107105 is the flattened triangle T(n,k)=C(n,k)*(C(n,k)+1)/2
 *            where T is read row‑by‑row. For a given index n, determine r =
 *            floor((sqrt(8n+1)-1)/2) (the row) and c = n - r*(r+1)/2 (the
 *            column). Then a(n) = C(r,c)*(C(r,c)+1)/2. The binomial
 *            coefficient is obtained with the provided binom() helper
 * Range:     u8 n <= 30, u16 n <= 70, u32 n <= 217, u64 n <= 681,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index c in this row */
    idx_t c = n - r * (r + 1) / 2;

    /* Compute binomial coefficient C(r,c) using the helper */
    seq_t comb = binom(r, c);

    /* Compute comb*(comb+1)/2 avoiding overflow when possible */
    seq_t val;
    if ((comb & 1) == 0)          /* comb is even */
        val = (comb / 2) * (comb + 1);
    else                          /* comb is odd */
        val = comb * ((comb + 1) / 2);

    return val;
}
