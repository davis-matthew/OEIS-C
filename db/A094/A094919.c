/*
 * A094919  a(n) = (4^n-1)/3 mod n.
 * https://oeis.org/A094919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((4^n - 1)/3) mod n
 * Range:     u8 n <= 263, u16 n <= 9991, u32 n <= 9991, u64 n <= 9991,
 *            u128 n <= 9991, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 4^n modulo 3*n; then (value-1)/3 modulo n */
    /* All intermediate arithmetic fits in 128 bits */
    unsigned __int128 N = n;               /* n as 128-bit */
    unsigned __int128 m = 3 * N;           /* 3*n as 128-bit */

    /* modular exponentiation r = 4^n mod m */
    unsigned __int128 r = 1 % m;
    unsigned __int128 base = 4 % m;
    idx_t e = n;

    while (e) {
        if (e & 1U)
            r = (r * base) % m;
        base = (base * base) % m;
        e >>= 1U;
    }

    /* r = 4^n mod (3*n), guarantee r % 3 == 1, so r-1 divisible by 3 */
    unsigned __int128 t = (r - 1) / 3;     /* (4^n - 1)/3 as 128-bit */
    unsigned __int128 ans = t % N;         /* modulo n */

    return (seq_t)ans;
}
