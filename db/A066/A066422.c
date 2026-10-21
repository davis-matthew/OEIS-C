/*
 * A066422  a(n) = least k such that phi^(k)(n) + 1 is prime, where phi^(k)
 *          denotes application of phi k times.
 * https://oeis.org/A066422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k ≥ 1 such that φ^k(n) + 1 is prime, where φ^k
 *            denotes the k‑fold application of Euler's totient
 * Range:     u8 n <= 958, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"        /* provides seq_t calc_phi(seq_t) */
#include "../utils/is_prime_big.h"    /* provides bool is_prime_big(seq_t) */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)                     /* below offset – undefined, return safe value */
        return 0;

    seq_t current = (seq_t)n;        /* φ^0(n) = n itself */
    seq_t k = 1;                    /* we look for φ^k(n), start at k = 1 */

    while (true) {
        current = calc_phi(current);        /* φ^k(n) */
        if (is_prime_big(current + 1)) {    /* check φ^k(n) + 1 */
            return k;                     /* smallest k found */
        }
        ++k;
    }
}
