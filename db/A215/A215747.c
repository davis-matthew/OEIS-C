/*
 * A215747  a(n) = (-2)^n mod n.
 * https://oeis.org/A215747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (-2)^n mod n
 * Range:     u8 n <= 258, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = (-2)^n mod n for n >= 1.
   Uses the provided big‑integer modular exponentiation function
   mod_pow(base, exp, mod).  For n <= 2 the base (-2) is congruent
   to 0 modulo n and therefore a(n) = 0.  For n > 2 the base
   reduces to n-2, which is the least non‑negative residue of -2. */
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* index zero is not used, but handle gracefully */

    seq_t sn = (seq_t)n;           /* communicate the modulus as seq_t */

    seq_t base = (n <= 2) ? 0 : (sn - 2);
    return mod_pow(base, n, sn);
}
