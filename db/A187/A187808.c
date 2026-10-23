/*
 * A187808  a(n) = |{0<=k<n: 2k+3, n(n-k)-1, n(n+k)-1 are all prime}|.
 * https://oeis.org/A187808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 995, u32 n <= 995, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A187808: a(n) = |{0 ≤ k < n : 2k+3, n(n‑k)‑1, n(n+k)‑1 are all prime}| */
/* The function counts the admissible k by testing each k and using the
   provided primality routine is_prime_big (for seq_t values) and
   is_prime (for idx_t values). */

#include <stdbool.h>
#include "../utils/is_prime.h"       /* provides is_prime(idx_t) */
#include "../utils/is_prime_big.h"   /* provides is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    seq_t count = 0;

    for (idx_t k = 0; k < n; ++k) {
        /* Small prime check: 2k+3 */
        idx_t p2k = 2 * k + 3;
        if (!is_prime(p2k))
            continue;

        /* Compute n(n‑k) – 1 and n(n + k) – 1 in seq_t type */
        seq_t sn = (seq_t)n;
        seq_t sk = (seq_t)k;

        seq_t prod1 = sn * (sn - sk) - 1;
        seq_t prod2 = sn * (sn + sk) - 1;

        if (is_prime_big(prod1) && is_prime_big(prod2))
            ++count;
    }

    return count;
}
