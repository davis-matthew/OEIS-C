/*
 * A130116  Inverse Moebius transform of a diagonalized matrix of A007436.
 * https://oeis.org/A130116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the triangle * T(i,j) = A007436(j)
 *            if j divides i, otherwise 0. * The triangle is read row‑by‑row
 *            starting with a(1)=1. * So for the index n: * let r be the
 *            smallest integer with r(r+1)/2 ≥ n; * let j = n - r(r-1)/2 and i
 *            = r. * Then a(n) = A007436(j) if i % j == 0, el...
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"   /* fib(idx_t) gives the nth Fibonacci number */

/*--------------- helper: Möbius function --------------*/
static int mobius(idx_t n)
{
    int cnt = 0;
    idx_t t = n;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
                if (e > 1) return 0;   /* square factor */
            }
            ++cnt;
        }
    }
    if (t > 1) ++cnt;               /* remaining prime factor */
    return (cnt % 2 == 0) ? 1 : -1;
}

/*--------------- helper: A007436(n)  ------------------*/
static seq_t a007436(idx_t n)
{
    /* b(n) = Σ_{d|n} μ(d) * F(n/d)   (F is Fibonacci) */
    __int128 sum = 0;  /* signed 128‑bit to handle additions/subtractions */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            int mu = mobius(d);
            if (mu != 0) {
                idx_t q = n / d;
                seq_t f = fib(q);
                if (mu == 1)
                    sum += ( __int128 )f;
                else
                    sum -= ( __int128 )f;
            }
            idx_t d2 = n / d;
            if (d2 != d) {
                int mu2 = mobius(d2);
                if (mu2 != 0) {
                    idx_t q2 = n / d2;   /* equals d */
                    seq_t f2 = fib(q2);
                    if (mu2 == 1)
                        sum += ( __int128 )f2;
                    else
                        sum -= ( __int128 )f2;
                }
            }
        }
    }
    /* sum is always non‑negative for the required range */
    return (seq_t)sum;
}

/*----------------- main sequence function -------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety, offset starts at 1 */

    /* find the row r such that r(r+1)/2 ≥ n  (binary search) */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                       /* row number */
    idx_t prev = r * (r - 1) / 2;        /* total elements before row r */
    idx_t j = n - prev;                  /* column index in the row (1‑based) */
    idx_t i = r;                         /* row index (1‑based) */

    if (i % j != 0) return 0;            /* off‑diagonal entries are 0 */
    return a007436(j);                   /* value on the diagonal */
}
