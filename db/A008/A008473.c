/*
 * A008473  If n = Product (p_j^k_j) then a(n) = Product (p_j + k_j).
 * https://oeis.org/A008473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^e || n} (p + e)
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes A008473: a(n) = ∏ (p + e) where n = ∏ p^e */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;
    uint64_t m = n;
    seq_t result = 1;

    /* Factor 2 */
    if (m % 2 == 0) {
        uint64_t e = 0;
        while (m % 2 == 0) {
            m /= 2;
            e++;
        }
        result *= ((seq_t)2 + (seq_t)e);
    }

    /* Factor odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            result *= ((seq_t)p + (seq_t)e);
        }
    }

    /* Remaining prime factor > 1 */
    if (m > 1) {
        result *= ((seq_t)m + (seq_t)1);
    }

    return result;
}
