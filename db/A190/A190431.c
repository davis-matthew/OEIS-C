/*
 * A190431  a(n) = [(b*n+c)*r] - b*[n*r] - [c*r], where (r,b,c)=(golden
 *          ratio,3,1) and []=floor.
 * https://oeis.org/A190431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((3*n+1)*(1+sqrt(5))/2) - 3*floor(n*(1+sqrt(5))/2) -
 *            1 (valid for n >= 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 
 * Lower rational bound for sqrt(5): 682/305 < sqrt(5).
 * This gives at most a single unit error in floor(n*sqrt(5)) for all n.
 */
static const unsigned long long SQRT5_NUM = 682ULL;
static const unsigned long long SQRT5_DEN = 305ULL;

/* floor(n * sqrt(5)), using the lower bound 682/305 */
static inline unsigned long long floor_n_sqrt5(idx_t n) {
    /* compute floor(n * 682 / 305) */
    unsigned long long approx = (unsigned long long)((unsigned __int128)n * SQRT5_NUM / SQRT5_DEN);
    /* test whether one more is needed */
    unsigned long long next = approx + 1ULL;
    /* 5*n*n  >= (next)^2  ? */
    unsigned __int128 left  = (unsigned __int128)5 * n * n;
    unsigned __int128 right = (unsigned __int128)next * next;
    if (right <= left)
        return next;
    return approx;
}

/* floor(n * phi) where phi = (1 + sqrt(5)) / 2  */
static inline unsigned long long floor_n_phi(idx_t n) {
    unsigned long long s = floor_n_sqrt5(n);
    unsigned long long t = n + s;              /* n + floor(n*sqrt5) */
    return t / 2ULL;                           /* integer division */
}

seq_t a(idx_t n) {
    if (n == 0)            /* offset safety; not used in the problem */
        return 0;

    unsigned long long k   = 3ULL * n + 1ULL;      /* 3n+1 */
    unsigned long long fn  = floor_n_phi(n);       /* floor(n*phi)   */
    unsigned long long fk  = floor_n_phi(k);       /* floor((3n+1)*phi) */

    unsigned long long val = fk - 3ULL * fn - 1ULL; /* a(n) = fk - 3*fn - 1 */
    return (seq_t)val;                             /* fits in seq_t (0..3) */
}
