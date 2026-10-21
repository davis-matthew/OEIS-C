/*
 * A076556  Greatest prime divisor of n-th prime + n.
 * https://oeis.org/A076556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = gpf(p_n + n)
 * Range:     u8 n <= 47, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   get_prime, gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greatest prime divisor of the n-th prime plus n. */

#include <stdint.h>
#include "../utils/get_prime.h"         /* get_prime(idx_t k) */
#include "../utils/gpf.h"              /* gpf(uint64_t x) */

seq_t a(idx_t n)
{
    /* Get the n-th prime (1-indexed). */
    uint32_t pn = get_prime(n);

    /* Compute pn + n as 64‑bit to avoid overflow. */
    uint64_t sum = (uint64_t)pn + (uint64_t)n;

    /* Return the greatest prime divisor using the supplied helper. */
    return (seq_t)gpf(sum);
}
