/*
 * A163755  a(0)=1. For n>=1, write n in binary. Let b(n,m) be the length of
 *          the m-th run of 0's or 1's, reading right to left. Then a(n) =
 *          product{m=1 to M} p(m)^b(n,m), where p(m) is the m-th prime, and M
 *          is the number of runs of 0's and 1's in binary n.
 * https://oeis.org/A163755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = product_{m=1}^{M} p(m)^{b(n,m)} ; a(0)=1 where p(m) is
 *            the m‑th prime and b(n,m) is the length of the m‑th run of 0’s
 *            or 1’s in the binary representation of n, read from the least
 *            significant bit to the most significant
 * Range:     u8 n <= 16, u16 n <= 68, u32 n <= 681, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

static const unsigned int primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    idx_t temp = n;
    int current_bit = temp & 1;
    seq_t result = 1;
    idx_t run_index = 1;

    while (temp > 0) {
        /* Count length of current run */
        unsigned int len = 0;
        while (temp > 0 && ((temp & 1) == current_bit)) {
            ++len;
            temp >>= 1;
        }

        /* Multiply result by prime(run_index)^len */
        seq_t prime = (seq_t)primes[run_index - 1];
        for (unsigned int k = 0; k < len; ++k)
            result = mul_small(result, (unsigned int)prime);

        current_bit ^= 1;   /* switch run type */
        ++run_index;
    }

    return result;
}
