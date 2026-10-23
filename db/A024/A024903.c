/*
 * A024903  Numbers k such that 7*k - 4 is prime.
 * https://oeis.org/A024903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th positive integer k such that 7*k-4 is prime
 * Range:     u8 n <= 11, u16 n <= 837, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* The sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1, undefined for 0 */

    /* current index k and corresponding value 7*k-4 */
    seq_t k   = 1;                     /* start with k = 1  (7*1-4 = 3) */
    seq_t val = 7 * k - 4;             /* pre‑compute 7*k-4 */

    idx_t count = 0;                   /* how many suitable k's we have found */

    while (1) {
        if (is_prime_big(val)) {
            ++count;
            if (count == n) return k;
        }
        /* advance to the next k */
        ++k;
        val = val + 7;                 /* 7*(k+1)-4 = (7*k-4) + 7 */
    }
}
