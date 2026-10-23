/*
 * A165993  a(n) = Sum_{j=1..prime(n)-1} floor(j^2/prime(n)).
 * https://oeis.org/A165993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..prime(n)-1} floor(j^2 / prime(n))
 * Range:     u8 n <= 10, u16 n <= 86, u32 n <= 9803, u64 n <= 9803,
 *            u128 n <= 9803, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* uses the supplied get_prime() helper */
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    uint32_t p = get_prime(n);          /* p = the nth prime */
    seq_t sum = 0;                      /* accumulate the sum in seq_t */
    for (idx_t j = 1; j < (idx_t)p; ++j) {
        /* compute floor(j^2 / p) using idx_t for the intermediate product */
        idx_t sq = j * j;
        sum += (seq_t)(sq / p);
    }
    return sum;
}
