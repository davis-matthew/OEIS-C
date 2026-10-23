/*
 * A032448  Smallest prime == -1 modulo prime(n).
 * https://oeis.org/A032448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime p with p ≡ -1 (mod prime(n)), i.e.,
 *            p+1 is a multiple of the n‑th prime
 * Range:     u8 n <= 13, u16 n <= 261, u32 n <= 542, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* undefined for n < 1 */
    uint32_t prime_n = get_prime(n);           /* n-th prime */
    seq_t q = (seq_t)prime_n;                  /* promote to seq_t */

    for (idx_t k = 1;; ++k) {
        seq_t candidate = (seq_t)k * q - (seq_t)1;  /* k*q - 1  */
        if (is_prime_big(candidate))
            return candidate;                   /* first prime found */
    }
}
