/*
 * A223585  T(n,k)=Number of nXk 0..3 arrays with antidiagonals unimodal.
 * https://oeis.org/A223585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are the pair enumerated by the Cantor
 *            diagonal enumeration of the 2‑D array T(n,k), T(n,k) =
 *            U(min(n,k))^(max(n,k)-min(n,k)+1) * ∏_{i=1}^{min(n,k)-1} U(i)^2,
 *            with U(t) = (t^6+15t^5+115t^4+405t^3+964t^2+660t+720)/720
 * Range:     u8 none, u16 n <= 11, u32 n <= 30, u64 n <= 69, u128 n <= 159,
 *            big any n
 * Helpers:   U_value, big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/U_value.h"

seq_t
a(idx_t n)
{
    if (n == 0) return 0;                 /* behaviour below offset is irrelevant */

    /* Find the diagonal s (the sum i+j) that contains the n‑th entry. */
    idx_t s = 2;                           /* smallest diagonal has sum 2 */
    while ((s - 1) * s / 2 < n) ++s;

    /* Position within the diagonal: offset in [0, s-2] */
    idx_t prev = (s - 2) * (s - 1) / 2;     /* elements before this diagonal */
    idx_t offset = n - prev - 1;            /* 0‑based offset */

    idx_t i = offset + 1;                   /* first coordinate */
    idx_t j = (s - 1) - offset;            /* second coordinate */

    idx_t m = (i < j) ? i : j;              /* min(i,j)  */
    idx_t M = (i > j) ? i : j;              /* max(i,j)  */

    /* Compute the power part U(m)^(M-m+1) */
    seq_t upower = big_pow(U_value(m), (seq_t)(M - m + 1));

    /* Compute the product ∏_{l=1}^{m-1} U(l)^2 */
    seq_t prod = 1;
    for (idx_t l = 1; l < m; ++l) {
        seq_t ul = U_value(l);
        prod *= ul;
        prod *= ul;                         /* multiply twice to get U(l)^2 */
    }

    return upower * prod;
}
