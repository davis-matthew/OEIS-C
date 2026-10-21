/*
 * A067831  Primes p such that sigma(p-6) < p.
 * https://oeis.org/A067831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Primes p such that sigma(p-6) < p
 * Range:     u8 n <= 27, u16 n <= 1745, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined offset, safety */

    idx_t count = 0;                      /* number of qualifying primes seen */
    idx_t p = 7;                          /* smallest prime > 6 */

    while (1) {
        if (is_prime(p)) {
            idx_t m = p - 6;              /* compute p-6 */
            seq_t sigma = sigma_of(m);    /* sigma(p-6) */
            if (sigma < (seq_t)p) {       /* check the inequality */
                count++;
                if (count == n) return (seq_t)p;
            }
        }
        p++;                              /* next integer to test */
    }
}
