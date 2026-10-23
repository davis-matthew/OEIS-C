/*
 * A147292  Eigentriangle.
 * https://oeis.org/A147292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  eigen, nonn, tabl
 * Formula:   a(n) = M(r,c) * Q(c) * where r = floor( (sqrt(8n+1)-1)/2 ), c =
 *            n - r(r+1)/2, * Q(0)=Q(1)=1, Q(k)=3*Q(k-1)-1 for k>=2, *
 *            M(r,c)=1 if r==c, * = fib( 2*(r-c)-1 ) otherwise * with
 *            fib(0)=0, fib(1)=1, fib(k)=fib(k-1)+fib(k-2)
 * Range:     u8 n <= 34, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   Q_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/Q_term.h"

static seq_t fib_n(idx_t m)
{
    if (m == 0) return (seq_t)0;
    if (m == 1) return (seq_t)1;
    seq_t a = (seq_t)0;
    seq_t b = (seq_t)1;
    for (idx_t i = 2; i <= m; ++i) {
        seq_t tmp = a + b;
        a = b;
        b = tmp;
    }
    return b;
}

seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    idx_t row_start = r * (r + 1) / 2;
    idx_t c = n - row_start; /* column index in the triangle */

    seq_t Qc = Q_term(c);

    seq_t Mrc;
    if (r == c) {
        Mrc = (seq_t)1;
    } else {
        idx_t d = r - c;                 /* distance below the diagonal */
        idx_t fib_idx = 2 * d - 1;       /* 1,3,5,... for d=1,2,3,... */
        Mrc = fib_n(fib_idx);
    }

    return Mrc * Qc;
}
