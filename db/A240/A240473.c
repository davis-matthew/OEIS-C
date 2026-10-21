/*
 * A240473  Distance from prime(n) to the closest smaller squarefree number.
 * https://oeis.org/A240473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000040(n) - A112925(n)
 * Range:     u8 n <= 54, u16 n <= 9088, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute the distance from the n‑th prime to the nearest smaller squarefree number */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined below offset, return 0 safely */

    uint32_t p = get_prime(n);      /* n-th prime (1‑based) */
    uint32_t m;

    /* search downward for the closest square‑free integer below the prime */
    for (m = p - 1; m > 0; --m) {
        if (is_squarefree(m)) break;
    }

    return (seq_t)(p - m);          /* distance = prime - nearest smaller square‑free number */
}
