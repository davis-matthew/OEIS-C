/*
 * A236531  a(n) = |{0 < k < n: {6*k -1 , 6*k + 1} and {prime(n-k), prime(n-k)
 *          + 2} are both twin prime pairs}|.
 * https://oeis.org/A236531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 < k < n : {6*k‑1 , 6*k + 1} and {prime(n‑k) ,
 *            prime(n‑k)+2} * are both twin prime pairs}|
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* provides uint32_t get_prime(idx_t k) */
#include "../utils/is_prime.h"     /* provides bool is_prime(idx_t n) */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;          /* a(1) = 0 */

    seq_t count = 0;

    for (idx_t k = 1; k < n; ++k)
    {
        /* Prime index on the right side of the definition. */
        idx_t pk = n - k;                  /* 1‑based index for get_prime */
        uint32_t p = get_prime(pk);        /* prime(n‑k) */

        /* Check the two twin‑prime pairs. */
        if (is_prime((idx_t)(6 * k - 1)) &&
            is_prime((idx_t)(6 * k + 1)) &&
            is_prime((idx_t)p) &&
            is_prime((idx_t)p + 2))
        {
            ++count;
        }
    }

    return count;
}
