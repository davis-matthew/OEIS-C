/*
 * A096561  Consecutive internal states of the second of the two linear
 *          congruential random number generators whose combined output is
 *          used in function RANDOM_NUMBER in version 8 of the Intel FORTRAN
 *          Compiler for Linux, using its intrinsic initialization.
 * https://oeis.org/A096561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2^31-250; a(n) = 40692 · a(n-1) mod (2^31-249)
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently: a(n) = (2^31-250) × 40692^(n-1)  (mod 2^31-249)   */

#include <stdint.h>   /* for uint64_t, uint32_t */

seq_t a(idx_t n) {
    /* Constants of the LCG */
    const seq_t mod  = ((seq_t)1 << 31) - 249ULL;   /* 2^31 - 249 = 2147483399 */
    const seq_t seed = mod - 1ULL;                 /* 2^31 - 250 = 2147483398 */
    const seq_t mult = 40692ULL;                   /* multiplier */

    if (n <= 1) return seed;                      /* a(1) */

    /* fast exponentiation of mult^(n-1) modulo mod */
    seq_t result_pow = 1;
    seq_t base = mult % mod;
    idx_t e = n - 1;

    while (e) {
        if (e & 1) {
            /* multiply result_pow by base modulo mod using 64‑bit arithmetic */
            result_pow = (seq_t)((unsigned long long)result_pow *
                                 (unsigned long long)base % (unsigned long long)mod);
        }
        /* square base modulo mod */
        base = (seq_t)((unsigned long long)base *
                       (unsigned long long)base % (unsigned long long)mod);
        e >>= 1;
    }

    /* a(n) = seed × result_pow mod mod */
    seq_t ans = (seq_t)((unsigned long long)seed *
                        (unsigned long long)result_pow % (unsigned long long)mod);
    return ans;
}
