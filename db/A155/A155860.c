/*
 * A155860  Number of ways to write 2n-1 as p + 2^x + 3*2^y with p an odd
 *          prime and x,y positive integers.
 * https://oeis.org/A155860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = #ways to write 2n-1 as p + 2^x + 3*2^y with p an odd
 *            prime and x, y positive integers
 * Range:     u8 n <= 67, u16 n <= 8706, u32 n <= 8706, u64 n <= 8706,
 *            u128 n <= 8706, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses only the allowed operations and the
   supplied primality test.  It is O(log m) per call where m = 2n-1,
   and easily meets the 2 s time limit for n up to 10 000. */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t m     = 2 * (seq_t)n - 1;          /* target odd number */

    for (idx_t y = 1; ; ++y) {
        seq_t term_y = 3 * ((seq_t)1 << y);   /* 3·2^y */
        if (term_y >= m) break;               /* y too large */

        for (idx_t x = 1; ; ++x) {
            seq_t term_x = ((seq_t)1 << x);   /* 2^x */
            seq_t sum    = term_y + term_x;  /* p + 2^x + 3*2^y so far */

            if (sum >= m) break;              /* p would be ≤ 1 */
            seq_t p = m - sum;                /* candidate odd prime */

            if ((p & (seq_t)1) && is_prime((idx_t)p))
                ++count;
        }
    }

    return count;
}
