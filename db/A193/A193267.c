/*
 * A193267  The number 1 alternating with the numbers A006953/A002445 (which
 *          are integers).
 * https://oeis.org/A193267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all prime powers p^e, where p-1 divides n *
 *            (i.e., for each prime p dividing n with exponent e, include p^e
 *            in the product * iff (p-1) | n). The sequence alternates 1 on
 *            odd indices; on even indices * it equals the product of the
 *            above prime powers
 * Range:     u8 n <= 255, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; behaviour for 0 is irrelevant */
    seq_t result = (seq_t)1;
    idx_t temp = n;

    /* Factor 2 separately (p-1 = 1 always divides n) */
    if ((temp & 1) == 0) {
        idx_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;   /* temp /= 2 */
            ++e;
        }
        /* multiply result by 2^e */
        for (idx_t i = 0; i < e; ++i)
            result *= (seq_t)2;
    }

    /* Factor odd primes */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* include p^e only if (p-1) divides original n */
            if (n % (p - 1) == 0) {
                seq_t pk = (seq_t)1;
                for (idx_t i = 0; i < e; ++i)
                    pk *= (seq_t)p;
                result *= pk;
            }
        }
    }

    /* Remaining prime factor (if any) */
    if (temp > 1) {
        idx_t p = temp;              /* exponent is 1 */
        if (n % (p - 1) == 0)
            result *= (seq_t)p;
    }

    return result;
}
