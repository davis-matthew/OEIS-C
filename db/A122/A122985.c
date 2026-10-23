/*
 * A122985  Smallest composite number greater than n, relatively prime to n,
 *          such that a(n)-n is also composite.
 * https://oeis.org/A122985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   gcd, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for the smallest composite integer > n that is
   relatively prime to n and whose difference with n is also composite. */

#include "../utils/gcd.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    const seq_t one = (seq_t)1;
    seq_t cand = (seq_t)n + one;   /* start checking from n+1 */

    for (;;) {
        /* cand must be composite (not prime and >1) */
        if (cand > one && !is_prime_small(cand)) {
            /* coprime to n */
            seq_t g = gcd(cand, (seq_t)n);
            if (g == one) {
                seq_t diff = cand - (seq_t)n;
                /* diff must also be composite */
                if (diff > one && !is_prime_small(diff)) {
                    return cand;
                }
            }
        }
        cand = cand + one;   /* next integer */
    }
}
