/*
 * A011610  Legendre symbol (n,137).
 * https://oeis.org/A011610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,137), i.e. a(n) = n^68 (mod 137) with
 *            values 0, 1 or –1. For n divisible by 137 the result is 0,
 *            otherwise it is 1 if n is a quadratic residue mod 137 and –1
 *            otherwise
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute n^68 mod 137 without requiring seq_t to hold the modulus. */
    const uint32_t MOD   = 137u;
    const uint32_t EXP   = 68u;
    uint32_t       base = (uint32_t)(n % MOD);
    uint32_t       res  = 1u;

    uint32_t exp = EXP;
    while (exp)
    {
        if (exp & 1u)
            res = (uint32_t)((uint64_t)res * base % MOD);
        base = (uint32_t)((uint64_t)base * base % MOD);
        exp >>= 1;
    }

    /* Convert the modular residue to the Legendre symbol value. */
    if (res == 0u)
        return (seq_t)0;
    else if (res == 1u)
        return (seq_t)1;
    else /* res == 136 (= MOD-1) */
        return (seq_t)-1;
}
