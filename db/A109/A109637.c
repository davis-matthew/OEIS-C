/*
 * A109637  Numbers n such that the string 55n is prime.
 * https://oeis.org/A109637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer n for which 55 * 10^digits(n) + n is
 *            prime
 * Range:     u8 none, u16 n <= 11, u32 n <= 59, u64 n <= 2000, u128 n <= 2000,
 *            big any n
 * Helpers:   count_digits, isprime_big, pow10, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isprime_big.h"
#include "../utils/pow10.h"
#include "../utils/push_seq.h"
#include "../utils/count_digits.h"

seq_t a(idx_t n)
{
    /* Static cache of discovered terms */
    static seq_t *cache      = NULL;
    static idx_t cache_cap   = 0;
    static idx_t cache_size  = 0;
    static idx_t last_checked = 0;

    /* Return stored value if already computed */
    if (cache_size >= n)
        return cache[n - 1];

    /* Extend cache as needed */
    while (cache_size < n)
    {
        /* Next candidate integer */
        idx_t cand = last_checked + 1;
        last_checked = cand;

        /* Number of decimal digits in cand */
        idx_t d = count_digits(cand);

        /* Candidate number 55...n (concatenation) */
        seq_t m = (seq_t)55 * pow10(d) + (seq_t)cand;

        /* Check primality */
        if (isprime_big(m))
        {
            push_seq(&cache, &cache_cap, &cache_size, (seq_t)cand);
        }
    }

    return cache[n - 1];
}
