/*
 * A129218  Permutations with exactly 10 fixed points.
 * https://oeis.org/A129218
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n,10) * D(n-10) where D(m) = derangement number,
 *            D(0)=1, D(1)=0, D(m) = (m-1)*(D(m-1)+D(m-2)) for m≥2
 *            (permutations of n elements with exactly 10 fixed points)
 * Range:     u8 n <= 12, u16 n <= 14, u32 n <= 18, u64 n <= 25, u128 n <= 38,
 *            big any n
 * Helpers:   binom, derangement_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binom.h"
#include "../utils/derangement_upto.h"

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 10)                     /* values below offset are undefined */
        return (seq_t)0;

    idx_t m = n - 10;               /* number of non‑fixed positions */
    seq_t der = derangement_upto(m);/* D(n-10) */
    seq_t bin = binom(n, (idx_t)10);/* C(n,10) */
    return bin * der;
}
