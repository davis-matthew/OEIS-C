/*
 * A066438  a(n) = 7^n mod n.
 * https://oeis.org/A066438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^n mod n
 * Range:     u8 n <= 267, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiply two 64‑bit integers modulo mod, using 128‑bit accumulator to
   avoid overflow regardless of mod size                                                                 */
static inline uint64_t mul_mod_uint64(uint64_t x, uint64_t y, uint64_t mod)
{
    return (uint64_t)((__int128)x * y % mod);
}

seq_t a(idx_t n)
{
    /* OEIS offset starts at 1.  For n==0 return 0 to keep behavior defined. */
    if (n == 0)
        return (seq_t)0;

    uint64_t mod = (uint64_t)n;            /* modulus */
    uint64_t base = 7 % mod;              /* base reduced modulo n */
    uint64_t result = 1 % mod;            /* accumulation result */
    idx_t exp = n;                        /* exponent equals n itself */

    while (exp > 0)
    {
        if (exp & 1)
            result = mul_mod_uint64(result, base, mod);
        base = mul_mod_uint64(base, base, mod);
        exp >>= 1;
    }

    return (seq_t)(result % mod);
}
