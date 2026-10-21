/*
 * A066136  Primes are replaced by their local sequence number in A000040,
 *          while composites are replaced by their sequence number in A002808;
 *          (a kind of eigen- or home-indexing).
 * https://oeis.org/A066136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 322, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   compute_pi, isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A066136 – Local index of primes and composites
 *  Formula:  a(n) = pi(n)                    if n is prime
 *            a(n) = n - pi(n) - 1           otherwise
 *  (where pi(n) is the prime‑counting function)
 */

#include <stdbool.h>
/* Helper prototypes (provided by the build environment) */
#include "../utils/isPrime.h"
#include "../utils/compute_pi.h"

/*  Entry a(n) of sequence A066136  */
seq_t a(idx_t n)
{
    /* undefined for n < 1 – return 0  */
    if (n < 1)
        return (seq_t)0;

    /* a(1) = 0  */
    if (n == 1)
        return (seq_t)0;

    /* compute π(n) as a seq_t value */
    seq_t pi_n = compute_pi(n);

    if (isPrime(n))
        return pi_n;
    else
        return (seq_t)n - pi_n - (seq_t)1;
}
