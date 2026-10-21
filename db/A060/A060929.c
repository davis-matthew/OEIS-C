/*
 * A060929  Second convolution of Lucas numbers A000032(n+1), n >= 0.
 * https://oeis.org/A060929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*((5*n+4)*L(n+2)+(5*n-2)*L(n+1))/10 with * Lucas
 *            numbers L(k) defined by L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 30, u64 n <= 72, u128 n <= 160,
 *            big any n
 * Helpers:   fib_seq, fib_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses 128‑bit arithmetic for primitive seq_t and big‑int
 * operations for C++ builds. */

#include <stdint.h>
#include "../utils/fib_seq.h"
#include "../utils/fib_uint128.h"

#ifdef __cplusplus

#else

#endif

seq_t a(idx_t n)
{
    if (n == 0) return 1;
#ifdef __cplusplus
    seq_t Fn1, Fn2;        /* F(n+1), F(n+2) */
    fib_seq(n + 1, &Fn1, &Fn2);
    seq_t Ln1 = 2 * Fn2 - Fn1;          /* L(n+1) */
    seq_t Fn3 = Fn1 + Fn2;              /* F(n+3) */
    seq_t Ln2 = 2 * Fn3 - Fn2;          /* L(n+2) */
    seq_t coeff1 = 5 * n + 4;
    seq_t coeff2 = 5 * n - 2;
    seq_t term1 = coeff1 * Ln2;
    seq_t term2 = coeff2 * Ln1;
    seq_t sum = term1 + term2;
    seq_t num = (n + 1) * sum;
    return num / 10;
#else
    unsigned __int128 Fn1, Fn2;        /* F(n+1), F(n+2) */
    fib_uint128(n + 1, &Fn1, &Fn2);
    unsigned __int128 Ln1 = 2 * Fn2 - Fn1;    /* L(n+1) */
    unsigned __int128 Fn3 = Fn1 + Fn2;        /* F(n+3) */
    unsigned __int128 Ln2 = 2 * Fn3 - Fn2;    /* L(n+2) */
    unsigned __int128 coeff1 = 5 * (unsigned __int128)n + 4;
    unsigned __int128 coeff2 = 5 * (unsigned __int128)n - 2;
    unsigned __int128 term1 = coeff1 * Ln2;
    unsigned __int128 term2 = coeff2 * Ln1;
    unsigned __int128 sum = term1 + term2;
    unsigned __int128 num = (unsigned __int128)(n + 1) * sum;
    unsigned __int128 res128 = num / 10;
    return (seq_t)res128;
#endif
}
