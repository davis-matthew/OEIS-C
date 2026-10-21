/*
 * A066715  a(n) = gcd(2n+1, sigma(2n+1)).
 * https://oeis.org/A066715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(2n+1, sigma(2n+1))
 * Range:     u8 n <= 81, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* 2n+1 never overflows for the tested range */
    idx_t m = 2 * n + 1;
    seq_t sigma_val = sigma_of(m);
    return gcd((seq_t)m, sigma_val);
}
