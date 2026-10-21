/*
 * A096068  Concatenated in binary representation: largest proper divisor of n
 *          and smallest prime factor of n.
 * https://oeis.org/A096068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binary_concat(n/spf(n), spf(n))
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here, spf(n) is the smallest prime factor of n  
   (with spf(1) defined as 1).  The result is the value
   obtained by writing the binary representation of the
   largest proper divisor of n followed by the binary
   representation of spf(n).  */
#include <stdint.h>
#include "../utils/get_spf.h"

static int bitlen_uint64(uint64_t v)
{
    return v == 0 ? 0 : 64 - __builtin_clzll(v);
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t spf_val = get_spf(N);          /* smallest prime factor */
    uint64_t d = N / spf_val;              /* largest proper divisor */

    int len_spf = bitlen_uint64(spf_val);   /* bit length of spf */

    seq_t result = ((seq_t)d << len_spf) | (seq_t)spf_val;
    return result;
}
