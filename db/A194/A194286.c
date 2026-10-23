/*
 * A194286  Triangular array: g(n,k)=number of fractional parts (i*sqrt(2)) in
 *          interval [(k-1)/n, k/n], for 1<=i<=2n, 1<=k<=n.
 * https://oeis.org/A194286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of terms in the triangular array row-wise g(r,k) =
 *            #i ∈ [1,2r] with fractional part of i*sqrt(2) in [(k-1)/r , k/r]
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t N) {
    /* Find row r and column k for the linear index N (1‑based) */
    idx_t r = 0, t = 0;
    while (t < N) { r++; t += r; }          /* r = smallest with t(r) >= N   */
    idx_t k = N - ( (r-1)*r/2 );            /* column within the row (1…r)  */

    /* High‑precision constants for sqrt(2) ~ 1.41421356237309504880... */
    static const unsigned __int128 DEN = ((unsigned __int128)1 << 60);          /* 2^60           */
    static const unsigned long long NUM_APPROX = 14142135623730950488ULL;     /* floor(1.414…*10^19) */
    static const unsigned long long DEN_APPROX = 10000000000000000000ULL;     /* 10^19           */
    static const unsigned __int128 NUM = ( (__int128)DEN * NUM_APPROX ) / DEN_APPROX;

    /* Pre‑compute comparison bounds */
    unsigned __int128 row_u = r;
    unsigned __int128 col   = k;
    unsigned __int128 left  = (col-1) * DEN;
    unsigned __int128 right = col * DEN;

    /* Count i with fractional part of i*sqrt(2) in the prescribed interval */
    seq_t count = 0;
    for (idx_t i = 1; i <= 2*r; ++i) {
        unsigned __int128 prod  = (__int128)i * NUM;
        unsigned __int128 frac  = prod % DEN;                /* fractional part * DEN */
        if (frac * row_u >= left && frac * row_u < right) {
            ++count;
        }
    }
    return count;
}
