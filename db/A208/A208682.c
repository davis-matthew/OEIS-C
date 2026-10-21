/*
 * A208682  Smallest m such that 2^m+m^2 = 0 mod prime(n), or 0 if no such m.
 * https://oeis.org/A208682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A208682:
 *   a(n) = smallest m ≥ 0 such that 2^m + m^2 ≡ 0 (mod prime(n));
 *   returns 0 if no such m exists (this occurs when prime(n) ≡ 7 (mod 8)).
 */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime (1‑based). */
    uint32_t p32 = get_prime(n);
    /* Aliases to proper types for arithmetic. */
    seq_t p = (seq_t)p32;

    /* OEIS remark: a(n)=0 iff prime(n) ≡ 7 (mod 8). */
    if ((p32 & 7) == 7)
        return (seq_t)0;

    /* For exponentiation modulo p we can iteratively multiply by 2. */
    seq_t pow2 = 1 % p;          /* 2^0 mod p */
    /* Search m from 1 upward. The found m always fits in seq_t
       (for the tested ranges).  Our loop should terminate because
       for all primes not ≡ 7 (mod 8) the congruence has a solution. */
    for (idx_t m = 1; ; ++m) {
        pow2 = (pow2 * 2) % p;                           /* 2^m mod p */
        seq_t m2 = ((seq_t)m * (seq_t)m) % p;            /* m^2 mod p */
        if ( (pow2 + m2) % p == 0 )
            return (seq_t)m;
        /* Safety guard: if m grows too large return 0 (unlikely). */
        /* According to the OEIS property this branch should
           never be reached for primes not ≡ 7 (mod 8). */
        if (m > (idx_t)(10 * (idx_t)p32))
            return (seq_t)0;
    }
}
