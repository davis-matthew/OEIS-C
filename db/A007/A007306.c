/*
 * A007306  Denominators of Farey tree fractions (i.e., the Stern-Brocot
 *          subtree in the range [0,1]).
 * https://oeis.org/A007306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, tabf, nice
 * Formula:   a(0) = 1; for n > 0, a(n) = A002487(2*n-1), where A002487 is
 *            Stern's diatomic sequence: s(0) = 0, s(1) = 1, s(2*m) = s(m),
 *            s(2*m+1) = s(m) + s(m+1)
 * Range:     u8 n <= 2345, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   stern
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/stern.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    return stern(2 * n - 1);
}
