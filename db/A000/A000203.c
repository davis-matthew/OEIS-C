/*
 * A000203  a(n) = sigma(n), the sum of the divisors of n. Also called
 *          sigma_1(n).
 * https://oeis.org/A000203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, core, nonn, nice, mult
 * Formula:   a(n) = Product_{p^e || n} (p^(e+1)-1)/(p-1), computed as Product
 *            (1 + p + ... + p^e)
 * Range:     u8 n <= 107, u16 n <= 9709, u32 n <= 9709, u64 n <= 9709,
 *            u128 n <= 9709, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;

    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1;
            seq_t pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            result *= term;
        }
    }
    if (m > 1) {
        result *= ((seq_t)m + 1);
    }
    return result;
}
