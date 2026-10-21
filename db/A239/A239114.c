/*
 * A239114  Exponent of 2 in prime factorization (i.e., 2-adic valuation) of
 *          odd nonprimes A014076(n) + 1.
 * https://oeis.org/A239114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = exponent of 2 in (A014076(n)+1)
 * Range:     u8 n <= 74, u16 n <= 7740, u32 n <= 7740, u64 n <= 7740,
 *            u128 n <= 7740, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* helper: compute exponent of 2 dividing v */
static seq_t exponent_of_two(seq_t v)
{
    seq_t e = 0;
    while ((v & (seq_t)1) == 0) {
        v >>= 1;
        e += 1;
    }
    return e;
}

seq_t a(idx_t n)
{
    /* static cache of odd nonprime numbers */
    static seq_t *cache      = NULL;
    static idx_t count       = 0;   /* number of terms stored */
    static idx_t capacity    = 0;   /* allocated size of cache   */
    static idx_t next_candidate = 1;/* next odd number to test   */

    if (count < n) {
        /* enlarge storage as necessary */
        if (capacity < n) {
            idx_t new_cap = (capacity == 0) ? 16 : capacity << 1;
            while (new_cap < n) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            capacity = new_cap;
        }

        /* generate odd nonprimes until we have n terms */
        while (count < n) {
            bool nonprime;
            if (next_candidate == 1)
                nonprime = true;
            else
                nonprime = !is_prime(next_candidate);

            if (nonprime) {
                cache[count] = (seq_t)next_candidate;
                count++;
            }
            next_candidate += 2;   /* stay on odd numbers */
        }
    }

    seq_t val = cache[n - 1] + (seq_t)1;
    return exponent_of_two(val);
}
