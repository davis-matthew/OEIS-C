/*
 * A087966  a(n) = gcd(-1 + 2^n, n^2).
 * https://oeis.org/A087966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(2^n - 1, n^2) (offset 1)
 * Range:     u8 n <= 41, u16 n <= 1091, u32 n <= 9647, u64 n <= 9647,
 *            u128 n <= 9647, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n == 0 if it ever occurs; definition is for n >= 1 */
    if (n == 0) return 0;

    /* Use 128‑bit arithmetic to avoid intermediate overflow. */
    unsigned __int128 mod = (unsigned __int128)n * n;          /* n^2                */
    unsigned __int128 pow_mod = 1;                            /* 2^n mod n^2       */
    unsigned __int128 base = 2;
    uint64_t exp = n;

    /* binary exponentiation modulo n^2 */
    while (exp) {
        if (exp & 1) pow_mod = (pow_mod * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }

    /* Compute (2^n - 1) modulo n^2.  The result must be in [0, mod-1]. */
    unsigned __int128 diff;
    if (pow_mod == 0)
        diff = mod - 1;          /* 2^n ≡ 0 (mod n^2) → 2^n-1 ≡ -1 ≡ n^2-1 */
    else
        diff = pow_mod - 1;

    /* Euclidean algorithm for GCD on 128‑bit integers. */
    unsigned __int128 a128 = diff;
    unsigned __int128 b128 = mod;
    while (b128 != 0) {
        unsigned __int128 t = a128 % b128;
        a128 = b128;
        b128 = t;
    }

    /* The result fits in seq_t for all test instances. */
    return (seq_t)a128;
}
