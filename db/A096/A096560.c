/*
 * A096560  Consecutive internal states of the first of the two linear
 *          congruential random number generators whose combined output is
 *          used in function RANDOM_NUMBER in version 8 of the Intel FORTRAN
 *          Compiler for Linux, using its intrinsic initialization.
 * https://oeis.org/A096560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2147483562, a(n) = 40014*a(n-1) mod 2147483563 (for n >
 *            1)
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    // modulus and constants fit in 32 bits
    const uint64_t MOD  = 2147483563ULL;   /* 2^31 - 85 */
    const uint64_t MUL  = 40014ULL;
    const uint64_t INIT = 2147483562ULL;   /* 2^31 - 86 */

    if (n <= 1) return (seq_t)INIT;

    /* compute mul^(n-1) mod MOD */
    uint64_t exp   = (uint64_t)(n - 1);
    uint64_t base  = MUL;
    uint64_t pow   = 1ULL;

    while (exp) {
        if (exp & 1ULL)
            pow = (pow * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }

    uint64_t result = (INIT * pow) % MOD;
    return (seq_t)result;
}
