/*
 * A159296  a(n) is the smaller number in the pair (L,m) which minimizes the
 *          primes of the form L^2 + m^2 under the constraint L + m = 2n + 1.
 * https://oeis.org/A159296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the smaller number L of the pair (L, m) with L + m = 2n
 *            + 1 such that L^2 + m^2 is prime. Among all such pairs, the one
 *            for which this prime is minimal is chosen; the function returns
 *            L. (The algorithm simply searches the valid L in descending
 *            order from n down to 1.)
 * Range:     u8 n <= 11, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd, is_prime_big, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/gcd.h"          /* gcd(seq_t, seq_t) */
#include "../utils/is_prime_big.h" /* is_prime_big(seq_t) */
#include "../utils/seq_one.h"      /* seq_one(void) */

seq_t a(idx_t n)
{
    idx_t sum = 2 * n + 1;           /* required L + m */
    seq_t one = seq_one();           /* constant 1 in seq_t */

    for (idx_t L = n; L >= 1; --L) {
        idx_t m  = sum - L;
        seq_t Lt = (seq_t)L;
        seq_t Mt = (seq_t)m;

        /* coprime check, required for prime sum of squares */
        if (gcd(Lt, Mt) != one)
            continue;

        seq_t p = Lt * Lt + Mt * Mt;
        if (is_prime_big(p))
            return Lt;               /* L is the desired smaller part */
    }

    /* In theory this point is never reached. */
    return (seq_t)0;
}
