/*
 * A066440  a(n) = 9^n mod n.
 * https://oeis.org/A066440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9^n mod n Computation uses binary exponentiation with
 *            modular multiplication provided by the utility function mul_mod
 * Range:     u8 n <= 264, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_mod.h"

/* Helper: modular exponentiation with a seq_t modulus. */
static seq_t pow_mod_seq(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1U)
            result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; the sequence is undefined for n=0.
       For safety, return 0 if n==0. */
    if (n == 0)
        return 0;

    seq_t mod = (seq_t)n;
    return pow_mod_seq((seq_t)9, n, mod);
}
