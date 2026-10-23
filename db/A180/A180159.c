/*
 * A180159  a(n) = smallest number k such that six consecutive prime numbers
 *          prime(n), prime(n+1),...,prime(n+5) are divisors of k, k+1,...,
 *          k+5 respectively.
 * https://oeis.org/A180159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that for i = 0..5, k+i is divisible by
 *            prime(n+i). This follows directly from a sequence of six
 *            consecutive congruence conditions; we solve them with the
 *            Chinese Remainder Theorem
 * Range:     u8 none, u16 n <= 1, u32 n <= 11, u64 n <= 257, u128 n <= 10000,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* modular inverse of a modulo m (both < 2^64) */
static uint64_t modinv(uint64_t a, uint64_t m) {
    int64_t b = m, u = 1, v = 0;
    while (b) {
        int64_t t = a / b;
        int64_t tmp = a - t * b;  a = b;  b = tmp;
        int64_t tmpu = u - t * v; u = v;  v = tmpu;
    }
    if (a != 1) return 0;          /* no inverse, should not happen */
    if (u < 0) u += m;
    return (uint64_t)u;
}

seq_t a(idx_t n) {
    /* k ≡ 0      (mod prime(n))          */
    /* k+1 ≡ 0    (mod prime(n+1))        */
    /* k+2 ≡ 0    (mod prime(n+2))        */
    /* k+3 ≡ 0    (mod prime(n+3))        */
    /* k+4 ≡ 0    (mod prime(n+4))        */
    /* k+5 ≡ 0    (mod prime(n+5))        */

    seq_t R = 0;                                   /* current solution */
    seq_t M = (seq_t)get_prime(n);                  /* product of processed moduli */

    /* process the remaining 5 congruences */
    for (idx_t i = 2; i <= 6; ++i) {
        uint64_t pi = get_prime(n + i - 1);         /* prime(n+i-1) */
        uint64_t residue = (pi - (i - 1)) % pi;     /* -(i-1) modulo pi */

        uint64_t Rm = (uint64_t)(R % (seq_t)pi);    /* R mod pi */
        uint64_t Mm = (uint64_t)(M % (seq_t)pi);    /* M mod pi */

        uint64_t inv = modinv(Mm, pi);              /* inverse of M modulo pi */

        int64_t diff = (int64_t)residue - (int64_t)Rm;
        int64_t modInt = (int64_t)pi;
        /* t ≡ (residue - R) * inv (mod pi) */
        int64_t t = ((diff % modInt + modInt) % modInt) *
                    (int64_t)inv % modInt;

        R = R + M * (seq_t)t;
        M = M * (seq_t)pi;
    }

    return R;
}
