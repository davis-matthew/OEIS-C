/*
 * A224478  (16^(5^n) + (10^n)/2 - 1) mod 10^n: a sequence of trimorphic
 *          numbers ending (for n > 1) in 5.
 * https://oeis.org/A224478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (16^(5^n) + 10^n/2 - 1) mod 10^n Equivalent congruential
 *            form used below
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow2, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/power5.h"

/* Return the modular inverse of an odd number a modulo 2^n.
   Uses Newton iteration, which doubles precision each step. */
static seq_t inv_mod_pow2(seq_t a, idx_t n)
{
    /* a is odd, modulus is 2^n.  Return inverse modulo 2^n. */
    seq_t inv = 1;          /* inverse modulo 2  */
    idx_t bits = 1;         /* current number of bits for modulus */

    while (bits < n)
    {
        idx_t next_bits = bits << 1;       /* double the precision */
        if (next_bits > n) next_bits = n;  /* stop at required modulus */

        seq_t mask   = pow2(next_bits);          /* 2^next_bits  */
        seq_t a_mod  = a % mask;                 /* a modulo current modulus */
        seq_t prod   = (a_mod * inv) % mask;     /* a*inv   (mod mask) */
        seq_t two   = 2;                         /* constant 2  */
        seq_t tminus = (mask + two - prod) % mask; /* 2 - prod  (mod mask) */
        inv = (inv * tminus) % mask;            /* next approximation  */
        bits = next_bits;
    }

    /* Ensure result is reduced modulo 2^n.  For the built‑in types this
       is guaranteed by the loop, but for arbitrary‑precision we guard
       against any higher bits that might appear. */
    seq_t mod = pow2(n);
    if (inv >= mod) inv %= mod;
    return inv;
}

extern seq_t a(idx_t n)   /* external linkage required by the platform */
{
    /* n >= 1 (offset 1).  Compute 2^n and 5^n first. */
    seq_t pow2_n = pow2(n);          /* 2^n  */
    seq_t pow5_n = power5(n);        /* 5^n  */

    /* rhs = -(2^(n-1)+1) modulo 2^n  ==  2^(n-1) - 1  */
    seq_t rhs = pow2_n;
    rhs >>= 1;           /* 2^(n-1)   */
    rhs -= 1;            /* 2^(n-1) - 1 */

    /* Compute the inverse of 5^n modulo 2^n. */
    seq_t inv5 = inv_mod_pow2(pow5_n, n);

    /* t = rhs * inv5 (mod 2^n)   ;   0 <= t < 2^n  */
    seq_t t = (rhs * inv5) % pow2_n;

    /* a(n) = 5^n * t   ;   this is < 10^n = 2^n * 5^n  */
    seq_t ans = pow5_n * t;

    return ans;
}
