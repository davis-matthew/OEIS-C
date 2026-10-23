/*
 * A176083  a(n) = 2^(2n-2) mod prime(2n).
 * https://oeis.org/A176083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2n-2) mod prime(2n)
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined for n = 0; return 0 as sentinel */

    /* Exponent: 2n - 2  (fits in 64‑bit for all intended inputs) */
    uint64_t exp = (uint64_t)(2 * n - 2);

    /* Modulus: (2n)-th prime */
    uint32_t p32 = get_prime(2 * n);
    seq_t mod = (seq_t)p32;

    seq_t base = (seq_t)2;
    return mod_pow(base, exp, mod);
}
