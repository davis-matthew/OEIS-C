/*
 * A069639  Smallest composite k such that phi(k) > k*(1-1/n^2).
 * https://oeis.org/A069639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nextprime(n^2)^2
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1 – n≥1 in normal use */
    uint64_t nsq = (uint64_t)n * (uint64_t)n;
    uint64_t p   = next_prime(nsq);      /* smallest prime > n²              */
    seq_t  sp   = (seq_t)p;
    return sp * sp;                      /* return p²                          */
}
