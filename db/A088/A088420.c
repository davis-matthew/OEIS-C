/*
 * A088420  Number of primes in arithmetic progression starting with 3 and
 *          with d = 2n.
 * https://oeis.org/A088420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of primes in arithmetic progression starting with 3 and step 2*n;
   count stops when the next term is not prime. */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* sequence is offset 1: a(1) is the first term listed. */
    seq_t cnt = (seq_t)1;                 /* 3 is always prime */
    idx_t v1 = 3 + (idx_t)2 * n;          /* 3 + 2n */
    if (is_prime(v1)) {
        cnt += (seq_t)1;
        idx_t v2 = 3 + (idx_t)4 * n;      /* 3 + 4n */
        if (is_prime(v2))
            cnt += (seq_t)1;
    }
    return cnt;
}
