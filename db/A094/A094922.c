/*
 * A094922  a(n) = (7^n-1)/6 mod n.
 * https://oeis.org/A094922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 295, u16 n <= 9686, u32 n <= 9686, u64 n <= 9686,
 *            u128 n <= 9686, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A094922: a(n) = ((7^n – 1)/6) mod n */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 6*n fits in 64 bits for n ≤ 1e4; using unsigned long long for safety. */
    unsigned long long mod = 6ULL * n;          /* modulus is 6*n */
    unsigned long long base = 7ULL % mod;       /* base ≡ 7 (mod 6n) */
    unsigned long long r = 1ULL % mod;          /* r will accumulate 7^n mod 6n */

    /* binary exponentiation: compute 7^n mod 6n */
    idx_t e = n;
    while (e > 0) {
        if (e & 1ULL)      r = (r * base) % mod;
        base = (base * base) % mod;
        e >>= 1ULL;
    }

    /* (7^n – 1)/6 is integer; compute it safely modulo n.   */
    unsigned long long t = (r + mod - 1ULL) / 6ULL;  /* add mod to avoid underflow when r==0 */
    unsigned long long res = t % n;                 /* final result < n  */

    return (seq_t)res;
}
