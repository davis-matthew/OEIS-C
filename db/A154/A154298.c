/*
 * A154298  Primes of the form (1+...+m)/33 = A000217(m)/33, for some m.
 * https://oeis.org/A154298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154298: primes of the form T(m)/33 where T(m)=m(m+1)/2
   (triangular numbers).  The condition T(m)/33 integer reduces to
   m(m+1) divisible by 66, which only occurs for the finitely many
   m ≤ 66.  We generate the primes by inspecting those m and
   cache the results so that a(n) can be returned in O(1) time for any
   n ≥ 1.  The sequence is finite: a(1)=2, a(2)=7, a(3)=17, a(4)=67. */
#include <stddef.h>
#include "../utils/is_prime_generic.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Cache of the primes.  There are at most four of them. */
    static seq_t primes[4];
    static size_t count = 0;

    /* On first invocation build the cache. */
    if (count == 0) {
        for (idx_t m = 1; m <= 66; ++m) {
            /* triangular number T(m) = m(m+1)/2, which fits in idx_t */
            idx_t tri = m * (m + 1) / 2;
            /* We need tri divisible by 33. */
            if (tri % 33 == 0) {
                idx_t candidate = tri / 33;
                /* Test primality using the helper that accepts idx_t. */
                if (is_prime_generic(candidate)) {
                    primes[count++] = (seq_t)candidate;
                    if (count == 4)   /* all known primes; stop early */
                        break;
                }
            }
        }
    }

    /* Undefined below offset; for n > count return 0 as the sequence
       terminates after the last prime. */
    if (n == 0 || n > count)
        return (seq_t)0;

    return primes[n - 1];
}
