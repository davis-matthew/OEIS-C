/*
 * A107636  Lesser prime in pair prime(k) +/- k for some k.
 * https://oeis.org/A107636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(A064403(n)) - A064403(n)
 * Range:     u8 n <= 4, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   get_prime, isprime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/isprime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)              /* undefined for offset 0 – harmless */
        return 0;

    idx_t k = 1;              /* candidate index in the prime list */
    idx_t found = 0;          /* number of qualifying indices seen */

    while (found < n)
    {
        seq_t pk = get_prime((uint64_t)k);      /* p_k */
        seq_t sk = (seq_t)k;                    /* k as seq_t */
        seq_t low  = pk - sk;                   /* p_k - k */
        seq_t high = pk + sk;                   /* p_k + k */

        if (isprime_big(low) && isprime_big(high))
        {
            found++;
            if (found == n)
                return low;    /* a(n) = p_k - k */
        }

        k++;
    }

    /* unreachable, but keeps the compiler happy */
    return 0;
}
