/*
 * A067333  Convolution of Fibonacci F(n+1), n>=0, with F(n+5), n>=0.
 * https://oeis.org/A067333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((18*n+25)*F(n+1) + 11*(n+1)*F(n)) / 5
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-01)
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the identity a(n)=sum_{k=0..n} F(k+1)*F(n+5-k) */
/* where F(0)=0, F(1)=1.  The helper fib_pair returns F(n) and F(n+1). */
/* mul_small is used to multiply a seq_t by a small unsigned int. */

#include <stdlib.h>
#include "../utils/fib_pair.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* External linkage as required */
seq_t a(idx_t n)
{
    seq_t f_n, f_n1;
    fib_pair(n, &f_n, &f_n1);                /* f_n  = F(n)   */
                                             /* f_n1 = F(n+1) */

    /* Compute (18*n+25)*F(n+1) and 11*(n+1)*F(n) */
    seq_t term1 = mul_small(f_n1, (unsigned int)(18 * n + 25));
    seq_t term2 = mul_small(f_n,   (unsigned int)(11 * (n + 1)));

    seq_t sum = term1 + term2;                /* Numerator */

    return sum / (seq_t)5;                    /* Exact division by 5 */
}
