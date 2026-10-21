/*
 * A090634  Start with the sequence [1, 1/2, 1/3, ..., 1/n]; form new sequence
 *          of n-1 terms by taking averages of successive terms; repeat until
 *          reach a single number F(n); a(n) = denominator of F(n).
 * https://oeis.org/A090634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator(2*(1-1/2^n)/n)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 60, u128 n <= 122,
 *            big any n
 * Helpers:   powmod2_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/powmod2_mod.h"
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static seq_t pow2_seq(idx_t exp)
{
    seq_t v = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        v <<= 1;  /* multiply by 2 */
    return v;
}

seq_t a(idx_t n)
{
    /* n >= 1 */
    /* 2^n modulo n */
    idx_t mod_pow = powmod2_mod(n, n);          /* 2^n mod n */
    /* (2^n - 1) modulo n */
    idx_t r = (mod_pow + n - 1) % n;
    /* gcd(n, 2^n - 1) */
    idx_t d = gcd_idx(n, r);
    /* (n/d) * 2^(n-1)  */
    seq_t n1 = (seq_t)n / (seq_t)d;
    seq_t pow2 = pow2_seq(n - 1);
    return n1 * pow2;
}
