/*
 * A215653  a(n) = smallest positive m such that m^2 = 1+k*n with positive k.
 * https://oeis.org/A215653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m > 1 such that m*m ≡ 1 (mod n)
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is computed by direct search up to n+1, using a
 * modular multiplication routine that never overflows any
 * unsigned integer type (including uint64_t, uint128_t, or
 * arbitrary‑precision big integers). */
#include <stdbool.h>

/* General modular multiplication: (a*b) % mod */
static inline seq_t mul_mod(seq_t a, seq_t b, seq_t mod)
{
    seq_t res = 0;
    seq_t aa  = a % mod;   /* ensure aa < mod */
    seq_t bb  = b;

    while (bb)
    {
        if (bb & 1)
        {
            /* res = (res + aa) % mod */
            if (res >= mod - aa) res -= (mod - aa);
            else                res += aa;
        }
        /* aa = (aa * 2) % mod */
        if (aa >= mod - aa) aa -= (mod - aa);
        else                aa += aa;
        bb >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* offset is 1; for n=1 the b‑formula gives m=2. */
    if (n == 1) return (seq_t)2;

    /* Search for the least m > 1 with m*m ≡ 1 (mod n).
       The solution is always ≤ n+1 (m = n+1 always works),
       so we iterate up to n+1. */
    idx_t bound = n + 1;
    for (idx_t i = 2; i <= bound; ++i)
    {
        seq_t m   = (seq_t)i;
        seq_t rem = mul_mod(m, m, (seq_t)n);  /* m^2 mod n */
        if (rem == 1)
            return m;
    }
    /* The loop must terminate; this point is unreachable. */
    return 0;
}
