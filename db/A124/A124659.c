/*
 * A124659  Twin prime products minus 2.
 * https://oeis.org/A124659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = p_n*(p_n+2)-2 where p_n is the n‑th prime * that starts a
 *            twin prime pair (p_n, p_n+2)
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 860, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the n‑th term of A124659. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* sequence is 1‑based; return 0 for safety */
        return 0;

    idx_t count = 0;
    for (idx_t p = 3; ; p += 2) {   /* only odd numbers can be prime >2 */
        if (is_prime(p) && is_prime(p + 2)) {
            ++count;
            if (count == n) {
                seq_t pp = (seq_t)p;
                seq_t res = pp * (pp + (seq_t)2) - (seq_t)2;
                return res;
            }
        }
    }
}
