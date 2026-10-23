/*
 * A140120  Primes of the form prime(x)*prime(x+1) - (prime(x+1)-prime(x)).
 * https://oeis.org/A140120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)*prime(n+1) - (prime(n+1)-prime(n)) Returns the
 *            n-th prime value of this expression (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 12, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* use 64‑bit primes for the base calculations */
    uint64_t p_prev = 2;            /* first prime */
    uint64_t p_cur  = 3;            /* second prime */
    idx_t    found = 0;

    while (found < n) {
        seq_t prev = (seq_t)p_prev;
        seq_t cur  = (seq_t)p_cur;

        /* compute candidate */
        seq_t val = prev * cur - (cur - prev);

        /* keep only prime results */
        if (is_prime_big(val)) {
            ++found;
            if (found == n)
                return val;
        }

        /* advance to next pair of consecutive primes */
        p_prev = p_cur;
        p_cur  = next_prime(p_cur + 1);
    }

    /* unreachable - the loop returns once n is reached */
    return (seq_t)0;
}
