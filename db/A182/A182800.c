/*
 * A182800  Positions of composites in A167171.
 * https://oeis.org/A182800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the nth distinct‑prime semiprime in the
 *            sequence of all primes and distinct‑prime semiprimes
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   _smallest_prime_factor, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_smallest_prime_factor.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t  found = 0;          /* how many semiprimes we have seen */
    seq_t  pos   = 0;          /* current position in A167171 */

    for (seq_t m = 2; ; ++m) {              /* iterate all naturals */
        /* Check if m is prime */
        uint64_t sp = _smallest_prime_factor((uint64_t)m);
        if (sp == (uint64_t)m) {            /* prime */
            ++pos;
            continue;
        }

        /* m is composite.  Does it equal p*q with distinct primes? */
        seq_t  q = m / sp;
        if (sp != q && is_prime((idx_t)q)) {
            ++pos;          /* m belongs to A167171 */
            ++found;
            if (found == n)
                return pos; /* we found the nth semiprime */
        }
        /* otherwise m is a composite not counted in A167171,
           so we skip it without incrementing pos */
    }
}
