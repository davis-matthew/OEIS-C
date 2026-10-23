/*
 * A141546  Numbers whose abundance is 14.
 * https://oeis.org/A141546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^k * (2^{k+1} - 15), where p = 2^{k+1} - 15 is prime and
 *            k is the n-th integer for which that prime occurs
 * Range:     u8 none, u16 n <= 3, u32 n <= 6, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*-------------------------------------------------------------------------*/
/*  OEIS A141546 – Numbers whose abundance is 14                          */
/*  a(n) = 2^k * (2^{k+1} - 15)   with 2^{k+1} - 15 prime                 */
/*-------------------------------------------------------------------------*/

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n == 0 */

    idx_t found = 0;
    for (idx_t k = 4; /* infinite loop */ ; ++k) {
        /* 2^{k+1} - 15  */
        seq_t p = ((seq_t)1 << (unsigned long long)(k + 1)) - (seq_t)15;

        /* skip impossible cases (e.g. p <= 1) */
        if (p <= (seq_t)1) continue;

        if (is_prime_big(p)) {
            ++found;
            if (found == n) {
                /* 2^k * p  */
                return ((seq_t)1 << (unsigned long long)k) * p;
            }
        }
    }
}
