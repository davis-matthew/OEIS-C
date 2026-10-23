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
 * Helpers:   _bigomega_local, _tau_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/_bigomega_local.h"
#include "../utils/_tau_local.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t omega = _bigomega_local(n);   /* Ω(n) */
    if (omega == 0)                     /* Ω(1) = 0, no divisors */
        return 0;

    uint64_t om = (uint64_t)omega;      /* safe conversion */

    /* q = odd part of om (remove all factors of 2) */
    uint64_t q = om;
    while ((q & 1) == 0)
        q >>= 1;

    uint64_t tau_om  = _tau_local(om);
    uint64_t tau_q   = _tau_local(q);

    /* Even divisors = total divisors – odd divisors. */
    seq_t result = (seq_t)(tau_om - tau_q);
    return result;
}
