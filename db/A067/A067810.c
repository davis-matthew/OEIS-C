/*
 * A067810  Composite n such that sigma(2*n) < prime(n).
 * https://oeis.org/A067810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   get_prime, is_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A067810: a(n) = nth composite m such that σ(2*m) < p(m)
   (σ is the sum–of–divisors function, p(m) is the m‑th prime). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/get_prime.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n = 0, return 0 */
    idx_t need  = n;               /* number of terms required */
    idx_t found = 0;               /* how many we have found so far */

    /* start testing from the first composite number 4 */
    for (idx_t cand = 4; ; ++cand) {
        if (is_prime(cand)) continue;          /* skip primes */

        /* compute σ(2*cand) */
        idx_t two_cand = cand << 1;            /* 2*cand */
        seq_t sigma = sigma_of(two_cand);

        /* prime of index cand (p(cand)) */
        seq_t prime = (seq_t)get_prime(cand);

        if (sigma < prime) {
            ++found;
            if (found == need) {
                return (seq_t)cand;
            }
        }
    }
}
