/*
 * A092348  a(n) = sigma_3(n) - sigma_1(n).
 * https://oeis.org/A092348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma_k(n,3) - sigma_k(n,1)
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 1547, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    seq_t s3 = sigma_k(n, 3);
    seq_t s1 = sigma_k(n, 1);
    return s3 - s1;
}
