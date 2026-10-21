/*
 * A096531  Number of different squares created when a square sheet of paper
 *          is folded n times, the first time by one of the diagonal of the
 *          square and after by the median of the triangle.
 * https://oeis.org/A096531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -1 + p(2^⌈n/2-1⌉) + d(2^⌊n/2-2⌋), n > 3 p(x) =
 *            x(x+1)(2x+1)/6 , d(x) = x(4x+1)(4x-1)/3 a(1)=0, a(2)=0, a(3)=4
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiply two seq_t values using a wider type when available */
static inline seq_t mul_seq(seq_t a, seq_t b)
{
#if defined(__SIZEOF_INT128__) && !defined(_WIN32)
    return (seq_t)((unsigned __int128)a * b);
#else
    /* For smaller seq_t sizes the product still fits into a larger builtin
       type on common architectures (at least 64‑bit). */
    return (seq_t)((uint64_t)a * (uint64_t)b);
#endif
}

/* exponentiation by squaring – works for any seq_t that supports mul_seq */
static inline seq_t pow2_seq(uint64_t e)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    while (e)
    {
        if (e & 1U) result = mul_seq(result, base);
        base = mul_seq(base, base);
        e >>= 1U;
    }
    return result;
}

/* p(x) = x(x+1)(2x+1)/6, computed avoiding overflow */
static inline seq_t p_func(seq_t x)
{
    seq_t a = x;
    seq_t b = x + (seq_t)1;
    seq_t c = mul_seq(x, (seq_t)2) + (seq_t)1;

    /* divide by 2 – at least one of a or b is even */
    if (!(a & (seq_t)1)) a >>= 1;
    else if (!(b & (seq_t)1)) b >>= 1;
    else c >>= 1;                 /* only for completeness; c may be even */

    /* divide by 3 – guaranteed to be present in a,b or c */
    if (a % (seq_t)3 == (seq_t)0) a /= (seq_t)3;
    else if (b % (seq_t)3 == (seq_t)0) b /= (seq_t)3;
    else c /= (seq_t)3;

    return mul_seq(mul_seq(a, b), c);
}

/* d(x) = x(4x+1)(4x-1)/3, computed avoiding overflow */
static inline seq_t d_func(seq_t x)
{
    seq_t a = x;
    seq_t b = mul_seq(x, (seq_t)4) + (seq_t)1;
    seq_t c = mul_seq(x, (seq_t)4) - (seq_t)1;

    /* divide by 3 – guaranteed to be present */
    if (a % (seq_t)3 == (seq_t)0) a /= (seq_t)3;
    else if (b % (seq_t)3 == (seq_t)0) b /= (seq_t)3;
    else c /= (seq_t)3;

    return mul_seq(mul_seq(a, b), c);
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)0;
    if (n == (idx_t)2) return (seq_t)0;
    if (n == (idx_t)3) return (seq_t)4;

    /* n >= 4 */
    uint64_t e1 = (n - (idx_t)1) / (idx_t)2;        /* ceil(n/2 - 1) = floor((n-1)/2) */
    uint64_t e2 = (n - (idx_t)4) / (idx_t)2;        /* floor(n/2 - 2)   = floor((n-4)/2) */

    seq_t k1 = pow2_seq(e1);   /* 2^e1 */
    seq_t k2 = pow2_seq(e2);   /* 2^e2 */

    seq_t val = p_func(k1);
    val += d_func(k2);
    val -= (seq_t)1;
    return val;
}
