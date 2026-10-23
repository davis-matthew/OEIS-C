/*
 * A126997  a(n) = binomial(prime(4+n), prime(4)).
 * https://oeis.org/A126997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(prime(4 + n), prime(4))
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 19, u64 n <= 289, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial_choose7, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The 4th prime is 7.  The binomial coefficient is computed
   multiplicatively to avoid overflow when the intermediate values
   would exceed the width of seq_t.  For seq_t types that are
   arbitrary‑precision this works unchanged; for fixed‑width
   types the computation stays within the supported range for all
   terms that fit. */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/binomial_choose7.h"

seq_t a(idx_t n)
{
    /* Obtain the (4+n)th prime.  get_prime returns a 32‑bit integer,
       which is safely convertible to idx_t. */
    idx_t pk = (idx_t)get_prime(4 + n);

    /* Since the fourth prime is fixed (7), we can compute the
       binomial coefficient directly. */
    return binomial_choose7(pk);
}
