/*
 * A079476  First prime greater than or equal to phi(n^2).
 * https://oeis.org/A079476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = first prime greater than or equal to φ(n^2) (OEIS
 *            A079476)
 * Range:     u8 n <= 16, u16 n <= 246, u32 n <= 316, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;                    /* Undefined for n < 1, return 0 as a safe value */

    /* Compute φ(n^2) = n * φ(n) */
    seq_t phi_n = phi((idx_t)n);            /* φ(n) */
    seq_t phi_n_sq = (seq_t)n * phi_n;      /* φ(n^2) */

    /* Find the first prime >= φ(n^2) */
    seq_t candidate = phi_n_sq;
    while (!is_prime_big(candidate)) {
        candidate = candidate + 1;
    }
    return candidate;
}
