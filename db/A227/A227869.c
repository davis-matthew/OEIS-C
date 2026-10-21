/*
 * A227869  Composite numbers congruent to 7 (mod 30).
 * https://oeis.org/A227869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 958, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Composite numbers of the form 30·k + 7 that are not prime.
   a(n) = the n‑th composite number congruent to 7 (mod 30) (n ≥ 1). */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n = 0, return 0 */

    idx_t k = 0;                    /* current k in 30·k + 7 */
    idx_t count = 0;                /* number of composites found so far */

    while (1)
    {
        seq_t val = (seq_t)((uint64_t)30 * (uint64_t)k + 7);

        /* val > 1 is always true for k ≥ 0, but keep the guard for safety */
        if (val > (seq_t)1 && !is_prime_big(val))
        {
            count++;
            if (count == n)
                return val;
        }
        k++;
    }
}
