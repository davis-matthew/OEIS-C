/*
 * A061364  Pseudo-random numbers: a (very weak) pseudo-random number
 *          generator from the second edition of the C book.
 * https://oeis.org/A061364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(A096553(n+2)/65536) mod 32768
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the standard MS C/Watcom C linear‑congruential generator
 * with seed 1, multiplier 1103515245, increment 12345, modulus 2^31.
 * The generator returns the high 15 bits of the internal state.
 * a(0) corresponds to the first call after seeding with 1.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constants for the generator */
    const uint64_t MOD   = 1ULL << 31;          /* 2^31  */
    const uint64_t A     = 1103515245ULL;       /* multiplier */
    const uint64_t C     = 12345ULL;             /* increment  */
    const uint64_t SEED  = 1ULL;                /* initial state before first call */

    /* We need X_{n+1} where X_k = A * X_{k-1} + C (mod MOD) */
    idx_t exp = n + 1;                          /* exponent for the transform */

    /* Composition of transforms (mul, add):
     * applying (a2,s2) after (a1,s1) gives
     *   mul = a2*a1 mod MOD
     *   add = a2*s1 + s2 mod MOD
     */
    uint64_t mul   = 1ULL % MOD;   /* identity multiplier */
    uint64_t add   = 0ULL;          /* identity adder   */
    uint64_t b_mul = A % MOD;      /* base multiplier   */
    uint64_t b_add = C % MOD;      /* base adder        */

    while (exp > 0) {
        if (exp & 1ULL) {
            /* result = base ∘ result */
            uint64_t new_mul = (b_mul * mul) % MOD;
            uint64_t new_add = (b_mul * add + b_add) % MOD;
            mul = new_mul;
            add = new_add;
        }
        /* base = base ∘ base */
        uint64_t new_mul = (b_mul * b_mul) % MOD;
        uint64_t new_add = (b_mul * b_add + b_add) % MOD;
        b_mul = new_mul;
        b_add = new_add;

        exp >>= 1ULL;
    }

    /* Compute the state after n+1 iterations starting from SEED */
    uint64_t state = (mul * SEED + add) % MOD;

    /* Return the high 15 bits of the state (mod 32768) */
    return (seq_t)(state >> 16);   /* floor(state / 65536) */
}
