/*
 * A179976  a(n) = 2^(2n+1) mod (2n+1).
 * https://oeis.org/A179976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n+1) mod (2*n+1)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute the modulus m = 2*n + 1 */
    seq_t m = (seq_t)2 * (seq_t)n + (seq_t)1;
    /* Exponent is also 2*n + 1, as a 64‑bit unsigned */
    uint64_t exp = (uint64_t)2 * n + 1;
    /* Base for modular exponentiation */
    seq_t base = (seq_t)2 % m;
    seq_t result = 1 % m;
    while (exp) {
        if (exp & 1U)
            result = (result * base) % m;
        base = (base * base) % m;
        exp >>= 1;
    }
    return result;
}
