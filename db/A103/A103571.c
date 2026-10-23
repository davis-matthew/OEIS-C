/*
 * A103571  Sum of the (primes > 5 modulo 17).
 * https://oeis.org/A103571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = Σ_{k=1..n} (p_k mod 17), where p_k is the k‑th prime > 5.
 *            (a(1)=7)
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* out of range, returns 0 */

    static idx_t      next_candidate = 7;   /* first prime > 5 */
    static idx_t      count          = 0;   /* number of primes stored */
    static seq_t     *sums           = NULL;/* cumulative sums of residues   */
    static size_t    capacity       = 0;   /* allocated capacity of sums    */

    /* Generate primes until we have at least n of them */
    while (count < n)
    {
        idx_t cand = next_candidate;

        /* test primality using the provided routine */
        if (is_prime(cand))
        {
            /* grow storage if needed */
            if (count == capacity)
            {
                size_t new_cap = (capacity == 0) ? 4 : capacity * 2;
                seq_t *tmp = (seq_t *)realloc(sums, new_cap * sizeof(seq_t));
                if (!tmp) return 0;          /* allocation failure */
                sums    = tmp;
                capacity = new_cap;
            }

            /* add residue of this prime to cumulative sum */
            seq_t residue = (seq_t)(cand % 17U);
            sums[count] = (count == 0 ? residue : sums[count - 1] + residue);
            ++count;
        }

        next_candidate += 2;   /* next odd candidate > 5 */
    }

    return sums[n - 1];
}
