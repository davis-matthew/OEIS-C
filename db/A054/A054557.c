/*
 * A054557  Number of labeled pure 2-complexes on n nodes (0-simplexes) with 5
 *          2-simplexes and 10 1-simplexes.
 * https://oeis.org/A054557
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(n-3)*(n-4)*(61*n-299)/10
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 32, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* safe fallback for 128‑bit builds */

seq_t a(idx_t n) {
    if (n < 5) return 0;          /* below or equal the offset */

#if defined(__SIZEOF_INT128__)   /* use fast integer arithmetic if available */
    __int128 t = (__int128)n * (n - 1) * (n - 2) * (n - 3) * (n - 4);
    t = t * ((__int128)61 * n - 299);
    t /= 10;
    return (seq_t)t;
#else                            /* fallback: use the binomial expression */
    seq_t c5 = binomial(n, 5);    /* C(n,5)  */
    seq_t c6 = binomial(n, 6);    /* C(n,6)  */
    return 72 * c5 + 4392 * c6;   /* 72*C(n,5) + 4392*C(n,6) */
#endif
}
