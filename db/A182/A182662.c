/*
 * A182662  Number of ordered ways to write n = p + q with q > 0 such that p,
 *          3*(p + prime(q)) - 1 and 3*(p + prime(q)) + 1 are all prime.
 * https://oeis.org/A182662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1860, u32 n <= 3930, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A182662: number of ordered ways to write n = p + q with q > 0 such that
   p, 3*(p + prime(q)) - 1 and 3*(p + prime(q)) + 1 are all prime.
   The function uses a brute‑force search over all prime values of p < n.  */

/* Include required utilities */
#include <stdbool.h>
#include "../utils/is_prime_big.h"      /* for is_prime_big(seq_t)               */
#include "../utils/is_prime.h"          /* for is_prime(idx_t)                  */
#include "../utils/get_prime.h"         /* for get_prime(idx_t)                 */

seq_t a(idx_t n)
{
    seq_t cnt = 0;

    /* No positive q can be obtained for n < 3 (since p ≥ 2). */
    if (n < 3) return cnt;

    for (idx_t p = 2; p < n; ++p)                      /* p must be prime */
    {
        if (!is_prime(p)) continue;

        idx_t q = n - p;                               /* q > 0 automatically */
        if (q == 0) continue;                         /* safety check */

        uint32_t prime_q = get_prime(q);              /* prime(q) = q‑th prime */

        /* Compute 3*(p + prime(q)) */
        seq_t sum = (seq_t)p + (seq_t)prime_q;
        sum = sum * (seq_t)3;

        /* Check that sum-1 and sum+1 are prime */
        if (is_prime_big(sum - (seq_t)1) &&
            is_prime_big(sum + (seq_t)1))
        {
            cnt = cnt + (seq_t)1;   /* increment result */
        }
    }

    return cnt;
}
