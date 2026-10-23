/*
 * A123571  Smallest prime p such that p divides m^(m+1)+1, where m =
 *          (p-2n-1)/(2n).
 * https://oeis.org/A123571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that (p-2n-1)/(2n) = m and
 *            m^(m+1)+1 ≡ 0 (mod p)
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 38, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;                  /* convert to seq_t */
    for (idx_t k = 1; ; ++k) {
        /* p = 2 * n * k + 1  (k = m + 1) */
        seq_t p = (seq_t)2 * nseq * (seq_t)k + 1;
        if (!is_prime_big(p)) continue;    /* p must be prime */

        seq_t m = (seq_t)k - 1;            /* m = k - 1   */
        /* Check if m^(m+1) ≡ -1 (mod p) */
        if (mod_pow(m, (uint64_t)k, p) == p - 1)
            return p;
    }
}
