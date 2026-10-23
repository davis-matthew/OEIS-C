/*
 * A143334  Only odd and even version of Pascal's triangle sequence:
 *          t(n,m)=If[m*(n - m) == 0, 1, Mod[Binomial[n, m], 2]*Prime[n] + (1
 *          - Mod[Binomial[n, m], 2])*(Prime[n] + 1)].
 * https://oeis.org/A143334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   Flattened entries of the triangle t(n,m) defined by t(n,m) =
 *            If[m*(n-m)==0, 1, Mod[Binomial[n,m],2]*Prime[n] +
 *            (1-Mod[Binomial[n,m],2])*(Prime[n]+1) ]. For a term with offset
 *            1, the zero‑based index is (n-1). Let r =
 *            floor((sqrt(8*(n-1)+1)-1)/2) be the row, and c = (n-1) -
 *            r*(r+1)/2 be the column...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* zero‑based position in the flattened triangle */
    idx_t idx0 = n - 1;

    /* find the row r such that cumulative + (r+1) > idx0  */
    idx_t r = 0;
    idx_t cumulative = 0;            /* number of entries before row r */

    while (idx0 >= cumulative + r + 1) {
        cumulative += r + 1;
        r++;
    }

    /* column position inside row r */
    idx_t m = idx0 - cumulative;

    /* edge entries are always 1 */
    if (m == 0 || m == r)
        return (seq_t)1;

    /* Binomial coefficient C(r,m) is odd iff (m & (r-m)) == 0
       (Lucas theorem).  */
    bool odd = ((m & (r - m)) == 0);

    /* Prime value used in the triangle: r‑th prime.
       get_prime(k) returns the k‑th prime with k ≥ 1.  */
    seq_t prime = (seq_t)get_prime(r);   /* r ≥ 2 here */

    return odd ? prime : (prime + 1);
}
