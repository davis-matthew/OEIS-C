/*
 * A157218  Number of ways to write the n-th positive odd integer in the form
 *          p+2^x+7*2^y with p a prime congruent to 1 mod 6 and x,y positive
 *          integers.
 * https://oeis.org/A157218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 9487, u32 n <= 9487, u64 n <= 9487,
 *            u128 n <= 9487, big any n
 * Helpers:   is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Implementation of OEIS sequence A157218.
 * a(n) = number of representations of 2*n‑1 as
 *        p + 2^x + 7*2^y
 *        where p is prime ≡ 1 (mod 6) and x,y ≥ 1.
 */
#include "../utils/pow2.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* target odd integer 2n‑1 */
    seq_t target = ((seq_t)2 * n) - 1;
    seq_t count  = 0;

    /* loop over x with 2^x < target‑1  (so that p>0) */
    for (idx_t x = 1;; ++x)
    {
        seq_t powx = pow2(x);
        if (powx > target - 1) break;

        /* loop over y with 7*2^y < target‑1 */
        for (idx_t y = 1;; ++y)
        {
            seq_t powy       = pow2(y);
            seq_t seven_powy = (seq_t)7 * powy;
            if (seven_powy > target - 1) break;

            seq_t sum = powx + seven_powy;
            if (sum >= target) break;      /* sum grows with y */

            seq_t p = target - sum;
            if (p % 6 == 1 && is_prime_big(p))
                ++count;
        }
    }
    return count;
}
