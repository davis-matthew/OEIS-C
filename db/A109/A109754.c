/*
 * A109754  Matrix defined by: a(i,0) = 0, a(i,j) = i*Fibonacci(j-1) +
 *          Fibonacci(j), for j > 0; read by ascending antidiagonals.
 * https://oeis.org/A109754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(i,0) = 0, a(i,j) = i*Fibonacci(j‑1)+Fibonacci(j) for j>0 ; The
 *            sequence is read by ascending anti‑diagonals: for each d≥0
 *            output a(d,0), a(d‑1,1), … , a(0,d)
 * Range:     u8 n <= 116, u16 n <= 348, u32 n <= 1222, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   fib_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/fib_u.h"

seq_t a(idx_t n) {
    /* Find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2. */
    idx_t d;
    {
        long double r = sqrtl((long double)8.0L * (long double)n + 1.0L);
        r = floorl((r - 1.0L) / 2.0L);
        d = (idx_t)r;
        /* adjust for possible rounding errors */
        while (((__uint128_t)d * (d + 1)) / 2 > n) d--;
        while (((__uint128_t)(d + 1) * (d + 2)) / 2 <= n) d++;
    }

    idx_t prev = ((__uint128_t)d * (d + 1)) / 2;   /* number of terms before diagonal d */
    idx_t k = n - prev;                            /* position inside diagonal (0..d) */
    idx_t i = d - k;                               /* row index */
    idx_t j = k;                                   /* column index */

    if (j == 0) return (seq_t)0;

    seq_t fib_jm1 = fib_u(j - 1);
    seq_t fib_j   = fib_u(j);

    return ((seq_t)i) * fib_jm1 + fib_j;
}
