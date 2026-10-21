/*
 * A239350  Superprimorials squared.
 * https://oeis.org/A239350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} prime(k)^{2(n-k+1)} = (∏_{k=1..n} ∏_{i=1..k}
 *            prime(i))^2
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t P = (seq_t)1;   /* primorial ∏_{i=1..k} prime(i)   */
    seq_t R = (seq_t)1;   /* product of all primorials: R = ∏_{k=1}^n P_k  */

    for (idx_t k = 1; k <= n; ++k) {
        uint32_t pk = get_prime(k);   /* prime(k), 1‑indexed */
        P = P * (seq_t)pk;            /* update primorial */
        R = R * P;                    /* update R = ∏ P_k */
    }

    return R * R;   /* a(n) = R^2 */
}
