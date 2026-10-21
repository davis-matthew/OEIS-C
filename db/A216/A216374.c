/*
 * A216374  Number of ways to express the square of the n-th prime as the sum
 *          of four nonzero squares.
 * https://oeis.org/A216374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((prime(n)^2 + 4*prime(n) + 24)/48) for n > 1, and
 *            a(1) = 1. This counts the number of ways to write the square of
 *            the n-th prime as the sum of four non‑zero squares
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;      /* undefined for offset, but harmless */
    }
    if (n == 1) {
        return 1;      /* special‑case for the first prime 2 */
    }
    seq_t p = (seq_t)get_prime(n);          /* nth prime */
    seq_t v = p * p + 4 * p + (seq_t)24;    /* p^2 + 4p + 24   */
    return v / (seq_t)48;                   /* floor division */
}
