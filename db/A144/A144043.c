/*
 * A144043  a(2n-1) = 2*prime(n), a(2n) = prime(n) + prime(n+1).
 * https://oeis.org/A144043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2n-1) = 2*prime(n), a(2n) = prime(n)+prime(n+1)
 * Range:     u8 n <= 61, u16 n <= 7024, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* sequence is defined for n >= 1; offset 1 */
    if (n == 0) return (seq_t)0;   /* not used, but safe fallback */
    idx_t k = (n + 1) / 2;           /* n = 2k-1 or 2k */
    if (n & 1) {                     /* odd term: a(2k-1) = 2 * prime(k) */
        return (seq_t)2 * (seq_t)get_prime(k);
    } else {                         /* even term: a(2k)   = prime(k) + prime(k+1) */
        return (seq_t)get_prime(k) + (seq_t)get_prime(k + 1);
    }
}
