/*
 * A135023  (NextPrime[6*n] - PreviousPrime[6*n])/2.
 * https://oeis.org/A135023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (NextPrime[6*n] - PreviousPrime[6*n]) / 2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* the offset is 1; behaviour for n<1 is irrelevant */

    idx_t m = n * 6;                /* 6 * n as idx_t */

    uint64_t nextp = next_prime((uint64_t)m);  /* smallest prime > m   */

    /* find the greatest prime <= m */
    idx_t prevp = 0;
    for (idx_t j = m; j >= 2; --j) {
        if (is_prime(j)) {
            prevp = j;
            break;
        }
        /* j is unsigned; the loop guard j >= 2 guarantees no underflow */
    }

    seq_t diff = (seq_t)nextp - (seq_t)prevp;  /* diff is always even */
    return diff / 2;
}
