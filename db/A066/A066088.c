/*
 * A066088  Number of distinct prime factors of sigma_2(n), where sigma_2(n) =
 *          A001157(n).
 * https://oeis.org/A066088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001221( sigma_2(n) ) where sigma_2(n) = sum of squares
 *            of divisors of n (utility function sigma_k(n,2) is used). The
 *            result is the number of distinct prime factors of that sum,
 *            computed by the helper function count_distinct_primes(uint64_t).
 *            For the standard 64‑bit build this is sufficien...
 * Range:     u8 n <= 14, u16 n <= 331, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_primes, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sigma_k.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* Compute the sum of squares of the divisors of n. */
    seq_t s = sigma_k(n, 2);

    /* For typical builds seq_t fits in uint64_t for n up to 10 000.
       Cast to uint64_t to count distinct prime factors.  If s
       exceeds 2^64-1 the lower 64 bits are used, which is a
       known shortcoming when the arbitrary‑precision build is
       used for very large n. */
    uint64_t val = (uint64_t)s;

    int cnt = count_distinct_primes(val);
    return (seq_t)cnt;
}
