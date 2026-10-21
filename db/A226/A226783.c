/*
 * A226783  If n=0 (mod 5) then a(n)=0, otherwise a(n)=5^(-1) in Z/nZ*.
 * https://oeis.org/A226783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   If n ≡ 0 (mod 5) then a(n)=0, otherwise a(n) is the *
 *            multiplicative inverse of 5 modulo n, i.e. the least positive k
 *            * such that 5·k ≡ 1 (mod n)
 * Range:     u8 n <= 320, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Zero for multiples of 5. */
    if (n % 5 == 0) return 0;

    /* Extended Euclidean algorithm to find the inverse of 5 modulo n.
     * All arithmetic fits in built‑in unsigned __int128 except the
     * final cast to seq_t, which is guaranteed to be ≤ n.            */
    unsigned __int128 a = 5;
    unsigned __int128 b = n;
    __int128        x0 = 1, x1 = 0;     /* x0/x1 correspond to the
                                         * coefficients of a and b.    */

    while (b != 0) {
        unsigned __int128 q = a / b;
        unsigned __int128 r = a % b;

        __int128 new_x = x0 - (__int128)q * x1;
        x0 = x1;
        x1 = new_x;

        a = b;
        b = r;
    }

    /* At this point a == gcd(5,n) == 1, and x0 is the modular
     * inverse of 5 modulo n (possibly negative).           */
    __int128 inv = x0;
    if (inv < 0) {
        inv += (__int128)n;               /* make it positive */
    }

    return (seq_t)(uint64_t)inv;
}
