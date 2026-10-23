/*
 * A127448  Triangle T(n,k) read by rows: matrix product A054525 * A127648.
 * https://oeis.org/A127448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign, easy
 * Formula:   a(n) = k * μ(r/k) where (r,k) are the row and column of the *
 *            nth term of the triangle T(r,k)=k*μ(r/k) read by rows
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static inline uint64_t triangular_row(uint64_t pos)
{
    long double val = sqrtl(8.0L * (long double)pos + 1.0L);
    uint64_t r = (uint64_t)((val - 1.0L) / 2.0L);   /* underestimate */
    /* adjust to guarantee r(r+1)/2 >= pos and (r-1)r/2 < pos   */
    while ((unsigned __int128)r * (r + 1) / 2 < pos) ++r;
    while (r > 0 && (unsigned __int128)(r - 1) * r / 2 >= pos) --r;
    return r;
}

static inline int compute_mobius(uint64_t n)
{
    if (n <= 1) return 1;              /* μ(1) = 1, μ(0) not needed */
    int mu = 1;
    uint64_t d = n;
    for (uint64_t p = 2; p * p <= d; ++p) {
        if (d % p == 0) {
            int cnt = 0;
            do {
                d /= p;
                ++cnt;
            } while (d % p == 0);
            if (cnt > 1) return 0;     /* squared prime factor */
            mu = -mu;                  /* each distinct prime flips sign */
        }
    }
    if (d > 1) mu = -mu;               /* remaining prime factor */
    return mu;
}

seq_t a(idx_t n)
{
    uint64_t pos = n;
    uint64_t r = triangular_row(pos);                     /* row index  */
    uint64_t prev = ((unsigned __int128)(r - 1) * r) / 2;   /* terms before row r */
    uint64_t k = pos - prev;                              /* column index */
    if (r % k != 0) return 0;                             /* T(r,k)=0 if k∤r */
    uint64_t d = r / k;                                   /* divisor needed for μ */
    int mu = compute_mobius(d);
    seq_t val = (seq_t)k * (seq_t)mu;                     /* T(r,k)=k*μ(d) */
    return val;
}
