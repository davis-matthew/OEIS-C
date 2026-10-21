/*
 * A089621  n^4 + n-th prime.
 * https://oeis.org/A089621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^4 + p_n where p_n is the n‑th prime (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"   /* provides get_prime(idx_t) */
#include <stdint.h>
#include <stdlib.h>

static inline seq_t pow4(idx_t n) {
    seq_t x = (seq_t)n;
    return x * x * x * x;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* undefined for n < 1, return 0 */
    seq_t n4 = pow4(n);                  /* compute n^4 in seq_t */
    seq_t p  = (seq_t)get_prime(n);       /* nth prime, cast to seq_t */
    return n4 + p;
}
