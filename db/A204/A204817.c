/*
 * A204817  Final nonzero digit of n^n in base 7.
 * https://oeis.org/A204817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (n / 7^v(n))^n (mod 7), with v(n)=max e s.t. 7^e | n.
 *            a(0) is defined to be 1
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the final nonzero digit of n^n in base 7. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the special case n = 0 (0^0 convention). */
    if (n == 0)
        return 1;

    /* Remove all factors of 7 from n to obtain the base u. */
    idx_t tmp = n;
    while (tmp % 7 == 0)
        tmp /= 7;          /* tmp is now u, not divisible by 7 */

    /* Compute u^n (mod 7).  u mod 7 is sufficient because the modulus is prime. */
    seq_t base = (seq_t)(tmp % 7);   /* 1 ≤ base ≤ 6 */
    seq_t result = 1;
    seq_t mod = 7;
    seq_t power = base % mod;

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1)
            result = (result * power) % mod;
        power = (power * power) % mod;
        exp >>= 1;
    }

    return result;
}
