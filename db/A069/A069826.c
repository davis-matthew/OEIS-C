/*
 * A069826  Numbers k such that sigma(k^2-k-1) = k*(k+1).
 * https://oeis.org/A069826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = t^2 - 1 where t >= 3 is the n‑th integer such that both
 *            t^2 + t - 1 and t^2 - t - 1 are prime. The values k are exactly
 *            the solutions of sigma(k^2 - k - 1) = k*(k+1)
 * Range:     u8 n <= 7, u16 n <= 36, u32 n <= 40, u64 n <= 400, u128 n <= 400,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* OEIS offset 1 */

    /* Cache of discovered terms */
    static seq_t *cache = NULL;
    static size_t len   = 0;
    static size_t cap   = 0;
    static idx_t  t_max = 2;        /* last t that was examined */

    /* Initialise cache lazily */
    if (cap == 0) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (len < n) {
        ++t_max;
        /* Compute t^2, p1, p2 */
        seq_t t_val = (seq_t)t_max;
        seq_t t_sq  = t_val * t_val;
        seq_t p1    = t_sq + t_val - 1;
        seq_t p2    = t_sq - t_val - 1;

        /* Both must be prime */
        if (is_prime_big(p1) && is_prime_big(p2)) {
            /* Store k = t^2 - 1 */
            if (len == cap) {
                cap <<= 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = t_sq - 1;
        }
    }

    return cache[n - 1];
}
