/*
 * A121559  Final result (0 or 1) under iterations of {r mod (max prime p <=
 *          r)} starting at r = n.
 * https://oeis.org/A121559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7919, u16 n <= 7919, u32 n <= 7919, u64 n <= 7919,
 *            u128 n <= 7919, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A121559.
   a(n) = final 0 or 1 obtained by repeatedly replacing r with
   r mod (largest prime ≤ r), starting from r = n. */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t r = n;                           /* current value of the iteration */
    while (r > 1) {                        /* continue until r is 0 or 1 */
        idx_t p = r;
        while (!is_prime(p)) {             /* search for the largest prime ≤ r */
            p--;
        }
        r -= p;                            /* r mod p == r – p (Bertrand’s postulate) */
    }
    return (seq_t)r;                       /* final 0 or 1 */
}
