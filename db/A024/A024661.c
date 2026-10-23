/*
 * A024661  n written in fractional base 10/6.
 * https://oeis.org/A024661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n written in fractional base 10/6 (digits 0‑9)
 * Range:     u8 n <= 19, u16 n <= 89, u32 n <= 949, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    if (N == 0) return 0;

    /* Store digits (least significant first) */
    unsigned char digits[128];
    size_t len = 0;

    while (N != 0) {
        unsigned d = (unsigned)(N % 10);          /* current least significant digit */
        digits[len++] = (unsigned char)d;

        /* Compute next N: N = (N - d) * 6 / 10 without overflow */
        seq_t temp = N - d;
        seq_t q    = temp / 10;
        seq_t r    = temp % 10;
        N = q * 6 + (r * 6) / 10;
    }

    /* Assemble the decimal number from most significant digit to least */
    seq_t res = 0;
    for (long long i = (long long)len - 1; i >= 0; --i) {
        res = res * 10 + digits[i];
    }
    return res;
}
