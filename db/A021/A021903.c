/*
 * A021903  Decimal expansion of 1/899.
 * https://oeis.org/A021903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = (10^(n+1) / 899) mod 10 i.e. the (n+1)th decimal digit of
 *            1/899
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 10^(n+1) modulo 8990 (899 * 10).
       The last digit of ⌊10^(n+1)/899⌋ depends only on the
       remainder modulo 8990, because adding 8990 corresponds
       to adding 10 to that quotient. */
    const uint64_t MOD = 8990ULL;   /* 899 * 10 */
    uint64_t e = (uint64_t)n + 1;   /* exponent */
    uint64_t base = 10 % MOD;       /* base modulo MOD */
    uint64_t pow = 1 % MOD;         /* result of modular exponentiation */

    /* Binary exponentiation modulo MOD */
    while (e > 0) {
        if (e & 1U)
            pow = (pow * base) % MOD;
        base = (base * base) % MOD;
        e >>= 1;
    }

    /* floor(pow/899) is exactly the desired digit (0…9) */
    return (seq_t)(pow / 899);
}
