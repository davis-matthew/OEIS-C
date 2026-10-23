/*
 * A185657  Numbers n such that (n^2 + n + 41) / 43 is prime.
 * https://oeis.org/A185657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 109, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A185657: Numbers n such that (n^2 + n + 41)/43 is prime.
   Offset 1 (a(1)=41).  The value is found by enumerating
   candidates n ≡ 1 or 41 (mod 43) and testing whether
   (n^2 + n + 41)/43 is prime. */

#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t N)
{
    idx_t count = 0;                     /* how many terms found so far */
    seq_t n;                             /* current candidate */
    const unsigned residues[2] = { 1u, 41u };

    for (idx_t k = 0; ; ++k) {
        for (unsigned r = 0; r < 2; ++r) {
            n = (seq_t)residues[r] + (seq_t)k * 43;

            /* (n^2 + n + 41) is guaranteed to be divisible by 43 for these residues */
            seq_t num = n * n + n + 41;
            seq_t p   = num / 43;        /* exact division */

            if (is_prime_big(p)) {
                ++count;
                if (count == N)
                    return n;
            }
        }
    }
}
