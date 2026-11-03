/*
 * A063439  a(n) = phi(n)^phi(n).
 * https://oeis.org/A063439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n)^phi(n)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 198 (2026-11-01)
 * Helpers:   ipow, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    seq_t t = totient(n);
    return ipow(t, t);
}
