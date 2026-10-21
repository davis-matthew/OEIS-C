/*
 * A081204  Staircase on Pascal's triangle.
 * https://oeis.org/A081204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(ceil(n/2) + n, n)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 95,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a multiplicative binomial algorithm that is safe for
   both 64‑bit unsigned integers and arbitrary–precision integer types.
   It performs intermediate reductions with gcd and, when the result type
   is an unsigned 64‑bit integer, it uses 128‑bit arithmetic to avoid
   overflow.                                                          */

#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

/* Helper: multiply a * b and divide by d.  For builtin uint64_t
   types the product is performed in 128 bits, otherwise the regular
   seq_t operators are used. */
static inline seq_t mul_div(seq_t a, seq_t b, seq_t d)
{
#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
    __uint128_t mul = (__uint128_t)a * (__uint128_t)b;
    return (seq_t)(mul / d);
#else
    /* When seq_t is a big‑integer class the normal operators work */
    return (a * b) / d;
#endif
}

/* Safe binomial coefficient C(N,K) with k ≤ N; reduces intermediates
   with gcd to keep the size small.  Arguments are unsigned integer
   indices; the function returns a seq_t result. */
static seq_t binom_safe(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K; /* use symmetry */

    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i)
    {
        seq_t num = N - K + i;
        seq_t den = i;

        seq_t g = gcd(num, den);
        num /= g;  den /= g;

        g = gcd(res, den);
        res /= g;  den /= g;

        res = mul_div(res, num, den);   /* res = res * num / den */
    }
    return res;
}

/* Public entry point: a(n) as defined in OEIS A081204. */
seq_t a(idx_t n)
{
    /* ceil(n/2) */
    idx_t half = (n + 1) / 2;
    return binom_safe(n + half, half);
}
