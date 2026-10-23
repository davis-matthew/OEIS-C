/*
 * A177047  Least k>1 such that n^k starts with n in base 2.
 * https://oeis.org/A177047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = least k>1 such that the binary representation of n^k
 *            starts with the binary representation of n. The algorithm uses
 *            only integer arithmetic plus long‑double computations of
 *            fractional parts of logarithms, which is sufficient for n ≤
 *            10000
 * Range:     u8 n <= 38, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* Handle n = 0 (outside OEIS offset). */
    if (n == 0) {
        return 0;
    }

    /* Number of bits of n (i.e. floor(log2(n))+1). */
    unsigned L = bitlen_seq_t((seq_t)n);          /* 1 ≤ L ≤ 14 for n ≤ 10000 */

    /* Logarithm base 2 of n (as long double for high precision). */
    long double logn = logl((long double)n);
    long double log2 = logl((long double)2.0L);
    long double logn_div_2 = logn / log2;          /* log2(n) */
    long double frac0 = logn_div_2 - floorl(logn_div_2); /* fractional part */

    /* Constant 2^{L-1} used in the test. */
    unsigned long long top_mul = 1ULL << (L - 1); /* fits in 64 bits */

    for (idx_t k = 2; ; ++k) {
        /* Compute fractional part of k*log2(n). */
        long double f = frac0 * (long double)k;
        f -= floorl(f);   /* fractional part in [0,1) */

        /* Compute 2^f * 2^{L-1} and take the integer part. */
        long double prod = powl(2.0L, f + (long double)(L - 1));
        unsigned long long prefix = (unsigned long long)(floorl(prod));

        if ((seq_t)prefix == n) {
            return (seq_t)k;
        }
    }
}
