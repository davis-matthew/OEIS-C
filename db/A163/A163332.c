/*
 * A163332  Self-inverse permutation of the integers for constructing the
 *          Peano curve in an N X N grid.
 * https://oeis.org/A163332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A163327(A163333(A163327(n))) --- ternary-digit rule
 *            Computes the self‑inverse permutation A163332. Uses only integer
 *            arithmetic, no external helpers
 * Range:     u8 n <= 242, u16 n <= 9833, u32 n <= 9833, u64 n <= 9833,
 *            u128 n <= 9833, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Store at most 64 ternary digits; 3^40 ≈ 1.2×10^19 > 2^64-1. */
    uint8_t digits[64];
    size_t len = 0;
    idx_t temp = n;

    /* Extract ternary digits (least‑significant first). */
    if (temp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (temp > 0) {
            digits[len++] = (uint8_t)(temp % 3);
            temp /= 3;
        }
    }

    seq_t result = 0;
    seq_t pow3   = 1;      /* 3^j for current j */

    for (size_t j = 0; j < len; ++j) {
        int parity = 0;   /* parity of sum of digits at positions j+1, j+3, ... */

        for (size_t k = j + 1; k < len; k += 2) {
            if (digits[k] & 1)     /* only odd digits affect parity */
                parity ^= 1;
        }

        seq_t d        = (seq_t)digits[j];
        seq_t out_digit = parity ? ((seq_t)2 - d) : d;   /* complement if parity odd */

        result += out_digit * pow3;
        pow3   *= 3;
    }

    return result;
}
