/*
 * A054571  a(n) = phi(n - phi(n)), a(1) = 0.
 * https://oeis.org/A054571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n - phi(n))
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9475 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    /* a(1) is defined as 0 */
    if (n <= 1)
        return 0;

    /* step 1: φ(n) */
    seq_t phi_n = totient(n);

    /* step 2: n - φ(n)  (must stay ≥ 1 for n > 1) */
    seq_t m = (seq_t)n - phi_n;

    /* step 3: φ(n - φ(n)) */
    return totient((idx_t)m);
}
