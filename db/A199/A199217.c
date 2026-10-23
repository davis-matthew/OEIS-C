/*
 * A199217  Primes p in successive intervals (2^i+1 .. 3*2^(i-1)) such that p
 *          XOR (2^i -2) produces a prime q in the range (3*2^(i-1) ..
 *          2^(i+1)-1) for i=1,2,3...
 * https://oeis.org/A199217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A199217.  No closed formula; we search each successive
   interval until the n‑th qualifying prime is found.  The search
   is O(n) for n up to about 10 000, well below the time limit. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1, but protect zero input */

    idx_t count = 0;

    for (idx_t i = 1; ; ++i) {
        /* 2^i  (i ≥ 1)  */
        uint64_t pow_i = 1ULL << i;                /* 2^i  */
        uint64_t L     = pow_i + 1ULL;             /* 2^i + 1            */
        uint64_t R     = 3ULL * (pow_i >> 1);      /* 3*2^(i-1)          */
        uint64_t mask  = pow_i - 2ULL;             /* 2^i - 2             */
        uint64_t q_low = R;                        /* lower bound of q    */
        uint64_t q_high= (pow_i << 1) - 1ULL;      /* upper bound of q    */

        for (uint64_t p = L; p <= R; ++p) {
            if (!is_prime((idx_t)p)) continue;

            uint64_t q = p ^ mask;
            if (q < q_low || q > q_high) continue;
            if (!is_prime((idx_t)q)) continue;

            ++count;
            if (count == n) return (seq_t)p;
        }
    }
}
