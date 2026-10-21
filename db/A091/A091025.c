/*
 * A091025  Smallest positive k such that phi(1+k*2^m) <= phi(k*2^m) for all m
 *          = n (mod 12), where phi is Euler's totient function.
 * https://oeis.org/A091025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   crt_3_5_7, modinv_small, modpow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A091025:  a(n) is the smallest positive integer k such that
   φ(1 + k·2^m) ≤ φ(k·2^m) for all m ≡ n (mod 12).
   This holds iff 1 + k·2^m has prime factors 3, 5 and 7 for all
   such m.  Hence k must satisfy
        k·2^n ≡ -1 (mod p)   for p = 3, 5, 7.
   The solution for k is unique modulo 105 = 3·5·7, and the
   minimal positive residue is the desired term a(n).
   The value depends only on n mod 12, giving a period of 12.
*/

#include <stdint.h>
#include "../utils/crt_3_5_7.h"
#include "../utils/modinv_small.h"
#include "../utils/modpow2.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* reduce exponent modulo the order of 2 modulo each prime:
       order(2 mod 3) = 2, order(2 mod 5) = 4, order(2 mod 7) = 3 */
    uint64_t exp3 = n % 2ULL;
    uint64_t exp5 = n % 4ULL;
    uint64_t exp7 = n % 3ULL;

    /* compute 2^n mod each prime */
    uint64_t pow2_3 = modpow2(2ULL, exp3, 3ULL);
    uint64_t pow2_5 = modpow2(2ULL, exp5, 5ULL);
    uint64_t pow2_7 = modpow2(2ULL, exp7, 7ULL);

    /* k ≡ - (2^n)^{-1} (mod p) */
    uint64_t r3 = (3ULL - modinv_small(pow2_3, 3ULL)) % 3ULL;
    uint64_t r5 = (5ULL - modinv_small(pow2_5, 5ULL)) % 5ULL;
    uint64_t r7 = (7ULL - modinv_small(pow2_7, 7ULL)) % 7ULL;

    /* combine residues via CRT */
    uint64_t k = crt_3_5_7(r3, r5, r7);   /* 1 ≤ k ≤ 104 */

    return (seq_t)k;
}
