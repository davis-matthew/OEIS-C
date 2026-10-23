/*
 * A124697  Number of base 4 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 1 if n == 0 Lucas(n) + Lucas(2*n) otherwise Lucas(n) can
 *            be obtained from Fibonacci by Lucas(n) = 2*F(n+1) - F(n).
 *            Fibonacci is computed by fast doubling (O(log n))
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"

/* Fast doubling to compute Fibonacci pair (F(n), F(n+1)) */
static inline void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    seq_t a = 0;           /* F(0) */
    seq_t b = 1;           /* F(1) */

    /* Process bits from most significant to least significant */
    for (int i = (int)floor_log2_uint64(n); i >= 0; --i)
    {
        /* d = F(2k) = F(k) * (2*F(k+1) - F(k)) */
        seq_t two_b = b + b;
        seq_t two_b_minus_a = two_b - a;
        seq_t d = a * two_b_minus_a;

        /* e = F(2k+1) = F(k)^2 + F(k+1)^2 */
        seq_t e = a * a + b * b;

        a = d;
        b = e;

        /* If current bit of n is set, advance one step */
        if ((n >> i) & 1ULL)
        {
            seq_t c = a + b;
            a = b;
            b = c;
        }
    }
    *fn  = a;  /* F(n)  */
    *fn1 = b;  /* F(n+1) */
}

/* Compute Lucas number via Fibonacci */
static inline seq_t lucas(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);       /* fn  = F(n)   */
                                   /* fn1 = F(n+1) */
    seq_t two_fn1 = fn1 + fn1;    /* 2*F(n+1) */
    return two_fn1 - fn;          /* Lucas(n) = 2*F(n+1) - F(n) */
}

/* OEIS A124697 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t l_n  = lucas(n);
    seq_t l_2n = lucas(n * 2ULL);
    return l_n + l_2n;
}
