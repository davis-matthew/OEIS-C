/*
 * A134058  Triangle T(n, k) = 2*binomial(n, k) with T(0, 0) = 1, read by
 *          rows.
 * https://oeis.org/A134058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flatten(T(n,k)) where T(n,k)=2*binomial(n,k) (T(0,0)=1)
 * Range:     u8 n <= 39, u16 n <= 177, u32 n <= 609, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Rough estimate of the row containing index n */
    long double estimate = sqrt((long double)8.0 * (long double)n + 1.0);
    idx_t r = (idx_t)((estimate - 1.0L) / 2.0L);

    /* Correct any off‑by‑one errors in r */
    while (((unsigned __int128)r * (r + 1)) / 2 > n) --r;
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n) ++r;

    idx_t start = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    idx_t k = n - start;

    if (r == 0)
        return (seq_t)1;

    seq_t bin = binomial(r, k);
    return bin * (seq_t)2;
}
