/*
 * A220235  a(n) = (2^n + 3^n) modulo n.
 * https://oeis.org/A220235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n + 3^n) mod n
 * Range:     u8 n <= 271, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for offset 1, safe fallback */
    seq_t mod = (seq_t)n;              /* modulus as seq_t */
    seq_t p2   = mod_pow((seq_t)2, (uint64_t)n, mod);  /* 2^n mod n */
    seq_t p3   = mod_pow((seq_t)3, (uint64_t)n, mod);  /* 3^n mod n */
    return (p2 + p3) % mod;            /* (2^n + 3^n) mod n */
}
