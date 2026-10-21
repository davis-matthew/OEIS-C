/*
 * A064792  Append more digits to the n-th prime (leading zeros are permitted)
 *          until another prime is reached.
 * https://oeis.org/A064792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A030665(prime(n))
 * Range:     u8 n <= 9, u16 n <= 126, u32 n <= 45, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS A064792.
   The n-th prime p_n is taken, and the smallest prime larger
   than p_n that begins with the decimal representation of p_n
   (allowing leading zeros in the appended part) is returned. */

#include "../utils/get_prime.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"
#include <stdbool.h>

/* Note: seq_t, idx_t are defined by the build system. */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1. */
    seq_t prime = (seq_t)get_prime(n);

    /* Search for the smallest prime with prefix prime. */
    for (idx_t k = 1;; ++k) {
        seq_t base  = prime * pow10(k);          /* prime * 10^k   */
        seq_t end   = base + pow10(k) - (seq_t)1; /* last number with k digits appended */

        for (seq_t cand = base; cand <= end; ++cand) {
            if (is_prime_big(cand))
                return cand;
        }
    }
}
