/*
 * A176095  a(n) = n - phi(2*n), where phi() is the Euler totient A000010().
 * https://oeis.org/A176095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - phi(2*n)
 * Range:     u8 n <= 523, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 107 terms through n = 8926 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    idx_t two_n = n * 2;
    seq_t phi_val = totient(two_n);
    return ((seq_t)n) - phi_val;
}
