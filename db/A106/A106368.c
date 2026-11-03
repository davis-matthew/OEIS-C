/*
 * A106368  Necklaces with n beads of 6 colors, no 2 adjacent beads the same
 *          color.
 * https://oeis.org/A106368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 6 * a(n) = (1/n) * Sum_{d | n} φ(n/d) * (5*(-1)^d + 5^d),
 *            for n > 1 * * The implementation precomputes 5^d by fast
 *            exponentiation and φ(k) * by trial division. No external
 *            libraries are required; all * calculations use the user‑supplied
 *            types seq_t and idx_t
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 173 (2026-11-03)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/totient.h"

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

static seq_t pow5(idx_t exp)
{
    seq_t result = 1;
    seq_t base = 5;
    idx_t e = exp;
    while (e > 0)
    {
        if (e & 1) result = result * base;
        base = base * base;
        e >>= 1;
    }
    return result;
}

static seq_t term_for_divisor(idx_t d, idx_t n)
{
    idx_t nd = n / d;                 /* φ(n/d) */
    seq_t phi_val = totient(nd);
    seq_t pow5_val = pow5(d);          /* 5^d */
    seq_t coeff = (d & 1) ? (pow5_val - 5) : (pow5_val + 5);
    return phi_val * coeff;
}

seq_t a_wide(idx_t n)
{
    if (n == 1) return (seq_t)6;      /* base case */

    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            sum += term_for_divisor(d, n);
            idx_t d2 = n / d;
            if (d2 != d)
                sum += term_for_divisor(d2, n);
        }
    }
    return sum / (seq_t)n;
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
