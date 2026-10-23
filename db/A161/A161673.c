/*
 * A161673  Positions n such that A010060(n) + A010060(n+8) = 1.
 * https://oeis.org/A161673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 168, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th non‑negative integer n such that the binary
   parity of popcount(n) differs from the parity of popcount(n+8).
   Equivalently: let m = n >> 3; let t be the number of trailing 
   ones of m; then a(n) is defined for which t is even. */

#include <stdbool.h>

static inline bool good(idx_t n)
{
    /* Determine if popcount(n) parity differs from popcount(n+8). */
    idx_t m = n >> 3;      /* n divided by 8 */
    idx_t t = 0;           /* number of trailing ones in m */
    while (m & 1) {
        ++t;
        m >>= 1;
    }
    return (t & 1) == 0;   /* true when t is even */
}

seq_t a(idx_t n)
{
    idx_t idx   = 0;      /* current candidate for a(1), a(2), ... */
    idx_t found = 0;      /* how many good numbers found so far */

    while (found < n) {
        if (good(idx)) {
            ++found;
            if (found == n)
                return (seq_t)idx;   /* the n-th good number */
        }
        ++idx;
    }

    /* Unreachable – return 0 to satisfy type checking */
    return (seq_t)0;
}
