/*
 * A067548  Least k > n such that gcd(prime(n)+k, n+prime(k)) > 1.
 * https://oeis.org/A067548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > n such that gcd(prime(n)+k, n+prime(k)) > 1
 * Range:     u8 n <= 35, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1u) return (seq_t)0;          /* undefined for n < 1, but harmless */
    seq_t pn = (seq_t)get_prime(n);        /* prime(n) */

    for (idx_t k = n + 1u; ; ++k) {
        seq_t pk = (seq_t)get_prime(k);    /* prime(k) */
        seq_t v1 = pn + (seq_t)k;          /* prime(n) + k   */
        seq_t v2 = (seq_t)n + pk;          /* n + prime(k)   */
        if (gcd(v1, v2) > (seq_t)1) return (seq_t)k;
    }
}
