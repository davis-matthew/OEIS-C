/*
 * A127413  a(n) = Sum_{1<=k<=n, gcd(k,n)=1} prime(k).
 * https://oeis.org/A127413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = Sum_{1<=k<=n, gcd(k,n)=1} prime(k)
 * Range:     u8 n <= 16, u16 n <= 162, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety; sequence defined for n>=1 */

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (gcd((seq_t)k, (seq_t)n) == 1) {
            sum += (seq_t)get_prime(k);
        }
    }
    return sum;
}
