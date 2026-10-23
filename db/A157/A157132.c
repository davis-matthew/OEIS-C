/*
 * A157132  Factorial of primes divided by prime numbers' respective places in
 *          the sequence of primes.
 * https://oeis.org/A157132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n)!/n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   factorial, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS entry defines a(n) as the factorial of the n‑th prime
   divided by n.  The utilities provided by the build supply
   get_prime(k) – which returns the k‑th prime as a uint32_t – and
   factorial(m) – which returns m! as a seq_t.  We therefore only
   need to call these helpers and perform the final integer
   division.  No mutable state or memoisation is required because
   `factorial` is defined to compute the factorial directly in a
   correct and efficient way. */

#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Valid indices are 1‑based.  Any call with n==0 is out of
       range for the OEIS specification; return 0 to remain
       well‑defined. */
    if (n == 0)
        return (seq_t)0;

    /* Obtain the n‑th prime.  It fits in a uint32_t, but we cast
       to seq_t only after the factorial is computed. */
    uint32_t p = get_prime(n);

    /* Compute factorial of the prime.  factorial accepts an
       idx_t (uint64_t) so casting from uint32_t is safe. */
    seq_t f = factorial((idx_t)p);

    /* Divide by n (canonical cast to seq_t). */
    return f / (seq_t)n;
}
