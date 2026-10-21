/*
 * A083529  a(n) = 5^n mod 3*n.
 * https://oeis.org/A083529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^n mod (3*n)
 * Range:     u8 n <= 94, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_mod.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset 1 starts at n = 1 */

    seq_t mod_val = (seq_t)n * 3;        /* modulus 3*n; fits in seq_t for tested n */
    seq_t base = 5;
    seq_t res = 1 % mod_val;            /* start with 1, modulo is safe because n>0 */

    idx_t exp = n;
    while (exp)
    {
        if (exp & 1)
            res = mul_mod(res, base, mod_val);
        base = mul_mod(base, base, mod_val);
        exp >>= 1;
    }

    return res;
}
