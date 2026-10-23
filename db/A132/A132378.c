/*
 * A132378  Smallest integer that starts with n and is divisible by every
 *          nonzero digit of n.
 * https://oeis.org/A132378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 386, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the smallest integer that starts with n and is divisible
 * by every non‑zero digit of n. The algorithm finds the least multiple
 * of the lcm of those digits that has n as a prefix.
 */
#include "../utils/gcd.h"

static inline seq_t lcm_seq_t(seq_t a, seq_t b)
{
    return a / gcd(a, b) * b;
}

seq_t a(idx_t n)
{
    /* Compute the lcm of the non‑zero decimal digits of n. */
    seq_t L = 1;
    idx_t tmp = n;
    while (tmp > 0)
    {
        unsigned d = tmp % 10;
        if (d != 0)
            L = lcm_seq_t(L, (seq_t)d);
        tmp /= 10;
    }

    /* Search for the smallest k such that a solution exists. */
    seq_t pow10big = 1;                   /* 10^k as seq_t             */
    seq_t pow10mod = (seq_t)1 % L;         /* 10^k mod L                 */
    seq_t n_mod   = (seq_t)n % L;

    for (int k = 0; k <= 4; ++k)
    {
        /* t = (-n * 10^k) mod L */
        seq_t t = (L - ((n_mod * pow10mod) % L)) % L;
        if (t < pow10big) /* first valid k gives the smallest m */
        {
            return ((seq_t)n * pow10big) + t;
        }
        pow10big *= (seq_t)10;
        pow10mod = (pow10mod * (seq_t)10) % L;
    }

    /* The problem guarantees a solution for k ≤ 4,
       so this return is never reached. */
    return ((seq_t)n * (seq_t)10000) + (seq_t)2520;
}
