/*
 * A144521  Tetrahedral numbers k*(k+1)*(k+2)/6 such that exactly one of k,
 *          k+1, and k+2 is prime.
 * https://oeis.org/A144521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A144521.
   Formula: a(n) = k*(k+1)*(k+2)/6 where k is the (n-1)th integer
   such that exactly one of k, k+1, k+2 is prime. The offset is 1
   (a(1) corresponds to k = 0). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; find the nth qualifying k by brute force. */
    idx_t count = 0;
    idx_t k = 0;
    while (1)
    {
        /* Count how many of k, k+1, k+2 are prime. */
        int pc = (is_prime(k) ? 1 : 0)
              + (is_prime(k + 1) ? 1 : 0)
              + (is_prime(k + 2) ? 1 : 0);
        if (pc == 1)
        {
            ++count;
            if (count == n)
            {
                seq_t kk = k;
                /* compute the tetrahedral number safely in seq_t. */
                return kk * (kk + 1) * (kk + 2) / 6;
            }
        }
        ++k;
    }
}
