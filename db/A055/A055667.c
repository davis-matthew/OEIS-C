/*
 * A055667  Number of Eisenstein-Jacobi primes of norm n.
 * https://oeis.org/A055667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A055667: Number of Eisenstein-Jacobi primes of norm n
   Formula used: a(n) is 6 for n=3, 6 for n=p² with prime p≡2 (mod 3),
   12 for n=p with prime p≡1 (mod 3), and 0 otherwise. */

/* Utility: integer square root for 64‑bit integers */
static inline idx_t int_sqrt_u64(idx_t n) {
    if (n < 2) return n;
    idx_t r = 0;
    /* use successive halving of the candidate bits */
    idx_t bit = (idx_t)1 << 62;          /* highest power of 2 <= n*4 */
    while (bit > n) bit >>= 2;
    for (; bit != 0; bit >>= 2) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
    }
    return r;
}

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Small n that can never have a prime norm */
    if (n < 3) return (seq_t)0;

    /* Norm 3: unique ramified prime above 3 */
    if (n == 3) return (seq_t)6;

    /* If n is a rational prime */
    if (is_prime(n)) {
        if (n % 3 == 1) return (seq_t)12;   /* splits: two primes, 6 associates each */
        return (seq_t)0;                    /* primes with p ≡ 2 (mod 3) cannot have norm n */
    }

    /* Check for n = p² where p is prime and p ≡ 2 (mod 3) */
    idx_t r = int_sqrt_u64(n);
    if (r * r == n && is_prime(r) && r % 3 == 2) {
        return (seq_t)6;
    }

    /* All other n have no Eisenstein‑Jacobi prime of norm n */
    return (seq_t)0;
}
