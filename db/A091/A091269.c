/*
 * A091269  a(n) = length of longest prefix of binary representation of n that
 *          is also a suffix of that of n+1.
 * https://oeis.org/A091269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of longest prefix of binary(n) that is also a
 *            suffix of binary(n+1)
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return number of bits needed to represent v (at least 1). */
static inline unsigned bitlen_uint64(uint64_t v)
{
    return v == 0 ? 1 : 64 - __builtin_clzll(v);
}

seq_t a(idx_t n)
{
    uint64_t ln  = (uint64_t)n;
    uint64_t ln1 = ln + 1;

    unsigned Ls = bitlen_uint64(ln);
    unsigned Lt = bitlen_uint64(ln1);
    unsigned maxk = Ls < Lt ? Ls : Lt;

    for (unsigned k = maxk; k > 0; --k) {
        uint64_t pref = ln >> (Ls - k);

        uint64_t mask;
        if (k == 64)
            mask = (uint64_t)-1;
        else
            mask = ((uint64_t)1 << k) - 1;

        uint64_t suff = ln1 & mask;

        if (pref == suff)
            return (seq_t)k;
    }
    return 0;
}
