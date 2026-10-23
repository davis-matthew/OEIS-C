/*
 * A122988  Number of possible arrangements of the last three digits of x^n
 *          for all x>0 (leading zeros omitted).
 * https://oeis.org/A122988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of distinct residues modulo 1000 that are n‑th
 *            powers. The set of n‑th powers modulo 1000 equals the Cartesian
 *            product of the sets of n‑th powers modulo 8 and modulo 125,
 *            because 1000 = 8·125 and the Chinese Remainder Theorem gives a
 *            bijection between the residue classes. Therefore...
 * Range:     u8 n <= 0, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast modular exponentiation for small moduli */
static inline seq_t powmod(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    while (exp)
    {
        if (exp & 1U) result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* counts for modulus 8 */
    unsigned char used8[8] = {0};
    int cnt8 = 0;
    for (int x = 0; x < 8; ++x)
    {
        seq_t r = powmod((seq_t)x, n, 8);
        if (!used8[(unsigned int)r])
        {
            used8[(unsigned int)r] = 1;
            ++cnt8;
        }
    }

    /* counts for modulus 125 */
    unsigned char used125[125] = {0};
    int cnt125 = 0;
    for (int x = 0; x < 125; ++x)
    {
        seq_t r = powmod((seq_t)x, n, 125);
        if (!used125[(unsigned int)r])
        {
            used125[(unsigned int)r] = 1;
            ++cnt125;
        }
    }

    return (seq_t)((seq_t)cnt8 * (seq_t)cnt125);
}
