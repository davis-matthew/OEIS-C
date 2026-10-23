/*
 * A139187  Smallest twin prime member A001359 of the form k!/n-1.
 * https://oeis.org/A139187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = the smallest integer k!/n - 1 such that * (k!/n - 1) and
 *            (k!/n + 1) are twin primes. * This implementation searches k
 *            starting at 1 until a valid pair * is found, memoizing results
 *            for subsequent calls
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Cache for results up to 10000 (typical test range) */
static seq_t cache[10001];
static bool seen[10001] = {0};

seq_t a(idx_t n)
{
    /* Handle trivial invalid input - sequence defined for n >= 1 */
    if (n == 0)
        return 0;

    /* Return cached value if available */
    if (n <= 10000 && seen[n])
        return cache[n];

    seq_t fact = 1;              /* current factorial value (k!) */
    idx_t   mod  = 1 % n;        /* n modulo for divisibility test */

    for (idx_t k = 1; ; ++k)
    {
        /* Increment factorial: k! = (k-1)! * k */
        fact = fact * k;

        /* Update modulo progressively */
        mod = (mod * (k % n)) % n;

        /* Only interesting if n divides k! (i.e., mod == 0) */
        if (mod == 0)
        {
            /* Candidate twin prime:  p = fact / n - 1  */
            seq_t p  = fact / n - 1;
            seq_t p2 = p + 2;   /* upper twin prime */

            /* Check twin prime condition */
            if (is_prime_big(p) && is_prime_big(p2))
            {
                if (n <= 10000)
                {
                    cache[n] = p;
                    seen[n]  = true;
                }
                return p;
            }
        }
        /* The loop is guaranteed to terminate for all n ≥ 1
         * because the sequence is defined and an answer exists.
         */
    }
}
