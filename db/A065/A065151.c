/*
 * A065151  a(n) = prime(1 + A064722(n)).
 * https://oeis.org/A065151
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = prime( π( prime(n) ) - prime( π(n) ) + 1 )
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 2  (a(2) is the first term of the sequence) */

#include <stdint.h>
#include <stddef.h>

#include "../utils/compute_pi.h"
#include "../utils/get_prime.h"

/* Computes the n-th term of OEIS sequence A065151
 * for n >= 2.  The behavior for n < 2 is undefined.
 */
seq_t a(idx_t n)
{
    /* The sequence is defined in terms of prime indices.
       For n < 2 the offset is not defined; we simply return
       0 in that case (the caller should not use such values). */
    if (n < 2) {
        return (seq_t)0;
    }

    /* p_n  = prime(n)                      */
    seq_t p_n = get_prime((idx_t)n);

    /* π(n) = compute_pi(n)                 */
    seq_t pi_n = compute_pi(n);

    /* i = prime( π(n) )                     */
    seq_t i = get_prime((idx_t)pi_n);

    /* j = π( prime(n) )                     */
    seq_t j = compute_pi((idx_t)p_n);

    /* index = j - i + 1                      */
    seq_t idx = j - i + 1;

    /* a(n) = prime( index )                 */
    return get_prime((idx_t)idx);
}
