/*
 * A035160  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          -30.
 * https://oeis.org/A035160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-30,d) (OEIS A035160)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is fully self‑contained and uses only native C/C++
//   constructs.  No external utility headers are required.          */

#include <stdbool.h>
#include <stdint.h>

/* modular multiplication for 64‑bit operands, using __uint128_t to avoid overflow */
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* modular exponentiation: base^exp mod mod */
static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1) result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Legendre symbol ( -30 | p ) for an odd prime p.
   For p = 2,3,5 the symbol is 0 (handled separately).          */
static int legendre_minus30(uint64_t p)
{
    if (p == 2 || p == 3 || p == 5) return 0;          /* divisor of 30 */
    uint64_t a_mod = p - (30 % p);                    /* (-30) mod p */
    uint64_t res = mod_pow(a_mod, (p - 1) / 2, p);
    return (res == 1) ? 1 : -1;                       /* p does not divide a_mod */
}

/* OEIS sequence A035160 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* value not defined, but harmless */

    seq_t result = 1;
    idx_t tmp = n;

    /* Remove the factors 2, 3, 5 – they contribute the local factor 1. */
    while ((tmp & 1) == 0) tmp >>= 1;
    while (tmp % 3 == 0) tmp /= 3;
    while (tmp % 5 == 0) tmp /= 5;

    /* Factor the remaining part. */
    for (idx_t p = 7; p <= tmp / p; p += 2)
    {
        if (tmp % p == 0)
        {
            idx_t e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }

            int k = legendre_minus30(p);
            seq_t term;
            if (k == 0)               /* p = 2,3,5 – already skipped    */
                term = 1;
            else if (k > 0)           /* quadratic residue            */
                term = (seq_t)(e + 1);
            else                      /* non‑residue                  */
                term = (seq_t)((e & 1) ? 0 : 1);   /* 1 if e even, 0 if odd */

            result *= term;
            if (result == 0) break;   /* once zero, further factors are irrelevant */
        }
    }

    /* If a prime factor > sqrt(original n) remains. */
    if (result != 0 && tmp > 1)
    {
        int k = legendre_minus30(tmp);
        seq_t term;
        if (k == 0)           term = 1;
        else if (k > 0)       term = 2;     /* e = 1 ⇒ e+1 = 2            */
        else                  term = 0;     /* e = 1 ⇒ (1+(-1))/2 = 0    */
        result *= term;
    }

    return result;
}
