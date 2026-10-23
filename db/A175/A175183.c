/*
 * A175183  Pisano period of the 4-Fibonacci numbers A001076.
 * https://oeis.org/A175183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Pisano period of the 4‑Fibonacci sequence A001076 modulo n
 * Range:     u8 n <= 124, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence is F(0)=0, F(1)=1, F(k)=F(k-1)+4*F(k-2)   (k≥2)      */
/* The period is the smallest positive t such that the pair
   (F(t),F(t+1)) equals (0,1) modulo n.                  */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Compute the Pisano period of the recurrence modulo a single modulus */
static seq_t period_mod(seq_t mod)
{
    if (mod == (seq_t)1) return (seq_t)1;          /* period of modulus 1 is 1 */

    seq_t target1 = (seq_t)(1 % mod);              /* (0,1 mod mod) */
    seq_t a0 = (seq_t)0;
    seq_t a1 = target1;
    seq_t step = (seq_t)0;

    while (1) {
        seq_t a2 = (a0 + (seq_t)4 * a1) % mod;    /* next term */
        a0 = a1;
        a1 = a2;
        ++step;
        if (a0 == (seq_t)0 && a1 == target1) return step;
    }
}

/* Main function required by the entry */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;                  /* trivial case */

    seq_t lcm = (seq_t)1;                         /* accumulating lcm of prime‑power periods */

    /* Trial division factorisation of n */
    idx_t temp = n;
    for (idx_t p = 2; p <= temp / p; p = (p == 2) ? 3 : p + 2) {
        if (temp % p == 0) {
            /* count exponent of p */
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            /* modulus = p^exp */
            seq_t mod = (seq_t)1;
            for (idx_t k = 0; k < exp; ++k) mod *= (seq_t)p;
            seq_t t = period_mod(mod);
            seq_t g = gcd(lcm, t);
            lcm = (lcm / g) * t;                 /* lcm(lcm, t) */
        }
    }
    if (temp > 1) {                               /* remaining prime factor */
        seq_t mod = (seq_t)temp;
        seq_t t = period_mod(mod);
        seq_t g = gcd(lcm, t);
        lcm = (lcm / g) * t;
    }

    return lcm;
}
