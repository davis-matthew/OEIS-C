/*
 * A021140  Decimal expansion of 1/136.
 * https://oeis.org/A021140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 136) mod 10 * (nth decimal digit of
 *            1/136)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n-th decimal digit (0‑based) of 1/136. */
seq_t a(idx_t n)
{
    /* Exponent: 10^(n+1) */
    idx_t exp = n + 1;

    /* Compute 10^(exp) mod 1360 using binary exponentiation.
       1360 < 2^16, so 64‑bit arithmetic is safe. */
    uint64_t result = 1;          /* current power of 10 modulo 1360 */
    uint64_t base   = 10 % 1360;  /* 10 modulo 1360 */

    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % 1360;
        base   = (base * base) % 1360;
        exp >>= 1;
    }

    /* The digit is floor( result / 136 ).  result < 1360, so the
       quotient is a single decimal digit (0..9). */
    return (seq_t)(result / 136);
}
