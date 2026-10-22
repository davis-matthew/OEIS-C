/*
 * A005559  Number of walks on square lattice. Column y=1 of A052174.
 * https://oeis.org/A005559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = C(n+1,ceiling((n-1)/2))*C(n,floor((n-1)/2)) -
 *            C(n+1,ceiling((n-2)/2))*C(n,floor((n-2)/2))
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   a005559_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a005559_binom.h"

seq_t a(idx_t n)
{
    idx_t k1f = (n - 1) / 2;
    idx_t k1c = n / 2;
    seq_t term1 = a005559_binom(n + 1, k1c) * a005559_binom(n, k1f);
    seq_t term2 = 0;
    if (n >= 2) {
        idx_t k2f = (n - 2) / 2;
        idx_t k2c = (n - 1) / 2;
        term2 = a005559_binom(n + 1, k2c) * a005559_binom(n, k2f);
    }
    return term1 - term2;
}
