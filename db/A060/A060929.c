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
 * Validated: standard, pass, 43 terms through n = 974 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses 128‑bit arithmetic for primitive seq_t and big‑int
 * operations for C++ builds. */

#include <stdint.h>

/* C++ build: seq_t may be an arbitrary‑precision integer type. */
static void fib_seq(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) { *fn = 0; *fn1 = 1; return; }
    seq_t a, b;
    fib_seq(n >> 1, &a, &b);
    seq_t c = a * (b * 2 - a);
    seq_t d = a * a + b * b;
    if (n & 1) { *fn = d; *fn1 = c + d; }
    else { *fn = c; *fn1 = d; }
}

/* C build: use unsigned __int128 for safe intermediate arithmetic. */
static void fib_uint128(idx_t n, unsigned __int128 *fn, unsigned __int128 *fn1)
{
    if (n == 0) { *fn = 0; *fn1 = 1; return; }
    unsigned __int128 a, b;
    fib_uint128(n >> 1, &a, &b);
    unsigned __int128 c = a * ((b << 1) - a);
    unsigned __int128 d = a * a + b * b;
    if (n & 1) { *fn = d; *fn1 = c + d; }
    else { *fn = c; *fn1 = d; }
}

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
