/*
 * A177882  Trisection of A001317.
 * https://oeis.org/A177882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i: bit_i(3n)=1} (2^(2^i)+1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Product over all Fermat numbers whose index occurs in the binary
   representation of 3*n) */

#include <stdint.h>

/* local helper: compute 2^e in seq_t */
static seq_t pow2_big(idx_t e) {
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    while (e) {
        if (e & 1ull) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n) {
    idx_t m = n * 3ull;          /* compute 3*n */
    seq_t result = (seq_t)1;     /* identity for empty product */

    idx_t bit = 0;
    while (m) {
        if (m & 1ull) {
            /* Fermat number F_bit = 2^(2^bit) + 1 */
            seq_t f = pow2_big((idx_t)(1ull << bit));
            f = f + (seq_t)1;
            result = result * f;
        }
        m >>= 1;
        bit++;
    }
    return result;
}
