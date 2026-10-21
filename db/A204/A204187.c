/*
 * A204187  a(n) = Sum_{m=1..n-1} m^(n-1) modulo n.
 * https://oeis.org/A204187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{m=1..n-1} m^(n-1) (mod n)
 * Range:     u8 n <= 216, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)        /* a(1) = 0 */
        return 0;

    /* A204187 is known to be 0 for all n divisible by 4 */
    if ((n & 3ULL) == 0)   /* n % 4 == 0 */
        return 0;

    /* For prime n the sum (by Fermat's little theorem) is n-1 */
    if (is_prime(n))
        return (seq_t)(n - 1);

    seq_t mod  = (seq_t)n;
    seq_t sum  = 0;
    uint64_t exp = n - 1;          /* exponent fits in 64‑bit */

    for (idx_t m = 1; m < n; ++m) {
        seq_t term = mod_pow((seq_t)m, exp, mod);
        sum += term;
        if (sum >= mod)           /* sum < 2*mod after addition */
            sum -= mod;
    }
    return sum;
}
