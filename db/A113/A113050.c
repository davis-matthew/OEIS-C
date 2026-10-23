/*
 * A113050  a(1) = a(2) = 1; for n>2, a(n+1) = a(n) + a(n-1) iff n is prime,
 *          otherwise a(n+1) = a(n) + 1.
 * https://oeis.org/A113050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=1; for n>2, a(n)=a(n-1)+a(n-2) if (n-1) is prime,
 *            else a(n)=a(n-1)+1
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 131, u64 n <= 311,
 *            u128 n <= 719, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;              /* undefined for n==0, but return 0 safely */

    if (n == 1 || n == 2)
        return 1;              /* a(1) and a(2) are 1 */

    seq_t a_nm2 = 1;           /* a(k-2) */
    seq_t a_nm1 = 1;           /* a(k-1) */
    seq_t a_n = 1;             /* current a(k) */

    for (idx_t k = 3; k <= n; ++k) {
        if (is_prime((uint64_t)(k - 1)))
            a_n = a_nm1 + a_nm2;
        else
            a_n = a_nm1 + 1;

        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}
