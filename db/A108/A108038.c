/*
 * A108038  Triangle read by rows: g.f. = (x+y+x*y)/((1-x-x^2)*(1-y-y^2)).
 * https://oeis.org/A108038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = F(k+2)*F(n-k+2) - F(k+1)*F(n-k+1) (Fibonacci with
 *            F(0)=0, F(1)=1). a(n) is the entry read row‑wise from this
 *            triangle. The implementation computes the Fibonacci values by
 *            fast doubling and then evaluates the formula
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast doubling algorithm for Fibonacci numbers.
   The function fills fn  = F(n)    and fn1 = F(n+1). */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;                        /* a = F(k), b = F(k+1)   */
    fib_pair(n >> 1, &a, &b);          /* k = floor(n/2)         */
    seq_t two_b = b + b;
    seq_t c = a * (two_b - a);         /* F(2k) = a*(2*b - a)    */
    seq_t d = a * a + b * b;           /* F(2k+1) = a^2 + b^2    */
    if (n & 1) {                      /* odd n */
        *fn  = d;
        *fn1 = c + d;
    } else {                          /* even n */
        *fn  = c;
        *fn1 = d;
    }
}

seq_t a(idx_t n)
{
    /* find the row r such that (r+1)(r+2)/2 > n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* position k within the row */
    idx_t before = r * (r + 1) / 2;
    idx_t k      = n - before;   /* 0 <= k <= r */
    idx_t t      = r - k;        /* r-k = n-k   */

    /* compute the required Fibonacci values */
    seq_t F_k1, F_k2, F_t1, F_t2;
    fib_pair(k + 1, &F_k1, &F_k2);   /* F(k+1), F(k+2) */
    fib_pair(t + 1, &F_t1, &F_t2);   /* F(t+1)=F(r-k+1), F(t+2)=F(r-k+2) */

    /* T(r,k) = F(k+2)*F(r-k+2) - F(k+1)*F(r-k+1) */
    return F_k2 * F_t2 - F_k1 * F_t1;
}
