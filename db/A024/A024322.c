/*
 * A024322  a(n) = s(1)*t(n) + s(2)*t(n-1) + ... + s(k)*t(n+1-k), where k =
 *          floor((n+1)/2), s = A023531, t = (F(2), F(3), ...).
 * https://oeis.org/A024322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..floor((n+1)/2)} A023531(j) * A000045(n-j+2)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where A023531(j)=1 iff j = m*(m+3)/2 for some integer m≥1         */
/*  and A000045(k)=F(k) is the k‑th Fibonacci number (F(0)=0,F(1)=1)   */

#include <stdint.h>
#include "../utils/fib.h"   /* provides: seq_t fib(idx_t n) */

static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t k = (n + 1) >> 1;   /* floor((n+1)/2) */
    seq_t sum = 0;

    for (idx_t j = 1; j <= k; ++j) {
        /* test if j is of the form m*(m+3)/2  <=> 8*j+9 is a perfect square
           and (sqrt(8*j+9) - 3) is even                                       */
        idx_t d = 8 * j + 9;
        idx_t r = isqrt_u64(d);
        if ((r * r == d) && (((r - 3) & 1ULL) == 0)) {
            idx_t fib_index = n - j + 2;   /* F(n-j+2) */
            sum += fib(fib_index);
        }
    }
    return sum;
}
