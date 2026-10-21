/*
 * A065330  Largest divisor of n coprime to 6. I.e., a(n) = max { k |
 *          gcd(n,k)=k and gcd(k,6)=1 }.
 * https://oeis.org/A065330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy, changed
 * Formula:   a(n) = n with all factors of 2 and 3 removed * (equivalently,
 *            a(n) = n / GCD(n, 6^n))
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, not strictly required */

seq_t a(idx_t n)
{
    /* Start with the integer value of n (fits in seq_t). */
    seq_t res = (seq_t)n;

    /* Strip out all factors of 2. */
    while ((res % 2) == 0) {
        res /= 2;
    }

    /* Strip out all factors of 3. */
    while ((res % 3) == 0) {
        res /= 3;
    }

    return res;
}
