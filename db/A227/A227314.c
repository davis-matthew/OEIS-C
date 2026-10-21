/*
 * A227314  Number of prime factors, with multiplicity, of the sum of the
 *          first n composite numbers.
 * https://oeis.org/A227314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = bigomega( Σ_{k=1}^{n} composite_k ) (prime factor count
 *            with multiplicity of the sum of the first n composite numbers)
 * Range:     u8 n <= 16, u16 n <= 1766, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1 – behaviour for n<1 is unspecified */
        return 0;

    /* Static cache of prefix sums of composites */
    static seq_t *prefix = NULL;   /* prefix[i] = sum of first i+1 composites   */
    static size_t cap = 0;         /* current allocation capacity            */
    static idx_t count = 0;        /* number of composites stored             */
    static idx_t last_candidate = 3;/* last integer examined (starts before 4) */
    static seq_t cur_sum = 0;       /* running sum of composites               */

    /* Extend precomputation until we have at least n composites */
    while (count < n) {
        ++last_candidate;
        /* Skip 1‑3 */
        if (last_candidate < 4)
            continue;
        /* Skip primes */
        if (is_prime(last_candidate))
            continue;

        /* Found a composite */
        ++count;
        cur_sum = cur_sum + (seq_t)last_candidate;

        /* Grow storage if necessary */
        if (count > cap) {
            size_t new_cap = cap ? cap << 1 : 8;
            seq_t *tmp = (seq_t *)realloc(prefix, new_cap * sizeof(seq_t));
            if (!tmp) { abort(); }
            prefix = tmp;
            cap = new_cap;
        }

        /* Store the running sum */
        prefix[count - 1] = cur_sum;
    }

    /* Return the number of prime factors (with multiplicity) of the sum */
    return count_prime_factors(prefix[n - 1]);
}
