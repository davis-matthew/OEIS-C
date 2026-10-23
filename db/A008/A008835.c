/*
 * A008835  Largest 4th power dividing n.
 * https://oeis.org/A008835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = largest 4th power dividing n, * i.e. a(n) = ∏_{p^e‖n}
 *            p^(4⌊e/4⌋) . * The implementation factors n by trial division
 *            and multiplies the * appropriate 4‑th power factors
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    seq_t result = 1;
    if (n <= 1) return result;          /* a(1) = 1 */

    uint64_t nn = (uint64_t)n;

    /* Trial division by all potential prime factors ≤ √nn   */
    for (uint64_t p = 2; p <= nn / p; ++p) {
        if (nn % p != 0) continue;
        int e = 0;
        while (nn % p == 0) {
            nn /= p;
            ++e;
        }
        int exp4 = 4 * (e / 4);
        if (exp4 > 0) {
            seq_t pow_val = seq_pow((seq_t)p, (idx_t)exp4);
            result *= pow_val;
        }
    }

    /* Remaining factor > 1 (prime) has exponent 1 → contributes nothing. */

    return result;
}
