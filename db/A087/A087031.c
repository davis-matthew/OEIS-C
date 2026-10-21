/*
 * A087031  Numbers n such that 2p-n is prime, p is the smallest prime > n.
 * https://oeis.org/A087031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 5548, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A087031:
   numbers n such that 2*P - n is prime, where P is the smallest prime
   greater than n. */

#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t) */
#include "../utils/is_prime.h"     /* bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1.  For n = 0 nothing is
       defined; return 0 to keep the function well‑formed. */
    if (n == 0)
        return (seq_t)0;

    idx_t found = 0;
    seq_t value;

    for (idx_t cand = 1; ; ++cand) {
        uint64_t p   = next_prime(cand);             /* smallest prime > cand */
        uint64_t m   = 2 * p - cand;                 /* 2*p - cand  */
        if (is_prime((idx_t)m)) {                   /* test primality */
            ++found;
            if (found == n) {                       /* nth term reached */
                value = (seq_t)cand;
                return value;
            }
        }
    }
}
