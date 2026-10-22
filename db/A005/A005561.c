/*
 * A005561  Number of walks on square lattice. Column y=3 of A052174.
 * https://oeis.org/A005561
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = C(n+1, ceiling((n-3)/2))*C(n, floor((n-3)/2)) - C(n+1,
 *            ceiling((n-4)/2))*C(n, floor((n-4)/2)), i.e. with k = n-3: k =
 *            2m: a(n) = C(k+4, m) * (C(k+3, m) - C(k+3, m-1)), k = 2m+1: a(n)
 *            = C(k+3, m) * (C(k+4, m+1) - C(k+4, m))
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   bin_exact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bin_exact.h"

seq_t a(idx_t n) {
    idx_t k, m;
    if (n < 3) return 0;
    k = n - 3;
    if (k % 2 == 0) {
        seq_t d;
        m = k / 2;
        d = bin_exact(k + 3, m);
        if (m > 0) d -= bin_exact(k + 3, m - 1);
        return bin_exact(k + 4, m) * d;
    } else {
        seq_t d;
        m = (k - 1) / 2;
        d = bin_exact(k + 4, m + 1) - bin_exact(k + 4, m);
        return bin_exact(k + 3, m) * d;
    }
}
