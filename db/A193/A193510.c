/*
 * A193510  Number of even divisors of Omega(n).
 * https://oeis.org/A193510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of even divisors of Ω(n) (Ω = total number of
 *            prime factors counted with multiplicity)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9629 (2026-11-01)
 * Helpers:   bigomega, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/bigomega.h"
#include "../utils/num_divisors.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t omega = bigomega(n);   /* Ω(n) */
    if (omega == 0)                     /* Ω(1) = 0, no divisors */
        return 0;

    uint64_t om = (uint64_t)omega;      /* safe conversion */

    /* q = odd part of om (remove all factors of 2) */
    uint64_t q = om;
    while ((q & 1) == 0)
        q >>= 1;

    uint64_t tau_om  = num_divisors(om);
    uint64_t tau_q   = num_divisors(q);

    /* Even divisors = total divisors – odd divisors. */
    seq_t result = (seq_t)(tau_om - tau_q);
    return result;
}
