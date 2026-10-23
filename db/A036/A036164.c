/*
 * A036164  Log base 5 (n) mod 23.
 * https://oeis.org/A036164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = exponent k such that 5^k ≡ n (mod 23). For n ≡ 0 (mod 23)
 *            returns 0
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   log5_mod_23
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/log5_mod_23.h"

seq_t a(idx_t n)
{
    return log5_mod_23(n);
}
