/*
 * A062789  a(n) = gcd(n, phi(n) * (phi(n) + 1)).
 * https://oeis.org/A062789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = gcd(n, phi(n) * (phi(n) + 1))
 * Range:     u8 n <= 16, u16 n <= 316, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 100 terms through n = 1902 (2026-11-01)
 * Helpers:   gcd, totient_u64_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/totient_u64_to_seq.h"

seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;
    seq_t phi_val = totient_u64_to_seq(n);
    seq_t prod = phi_val * (phi_val + (seq_t)1);
    return gcd(n_val, prod);
}
