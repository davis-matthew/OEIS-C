/*
 * A170985  Carryless product n X n in base 4.
 * https://oeis.org/A170985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = carryless product of n with itself in base 4
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) ⇔ the carry‑less product of n with n (digits in base 4,
   addition and multiplication performed modulo 4 without carry).  The
   result is returned as a decimal integer in type seq_t.  The function
   works for any n >= 0 and for any seq_t/idx_t sizes (8‑128 bits).  For
   n up to 10 000 the running time is trivial.  The implementation
   uses only the operations permitted in the spec and is fully
   portable to C11 and C++17. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* we need no more than 32 digits for a 64‑bit input: log_4(2^64) = 32 */
    static const int MAX_DIGITS = 32;
    static const int MAX_E      = 64;          /* 2*MAX_DIGITS  */
    uint8_t digits[MAX_DIGITS];
    unsigned e[MAX_E];
    int len = 0;

    /* convert n to base‑4 digits (least significant first) */
    idx_t tmp = n;
    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > 0 && len < MAX_DIGITS) {
            digits[len++] = tmp & 3;   /* tmp % 4 */
            tmp >>= 2;                /* tmp /= 4 */
        }
    }

    /* compute carry‑less convolution of digits with itself, modulo 4 */
    for (int i = 0; i < MAX_E; ++i) e[i] = 0;
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < len; ++j) {
            e[i + j] += digits[i] * digits[j];
        }
    }
    for (int k = 0; k < MAX_E; ++k) {
        e[k] %= 4;
    }

    /* find the most significant non‑zero digit (may be 0 for n = 0) */
    int msd = MAX_E - 1;
    while (msd > 0 && e[msd] == 0) msd--;

    /* convert digit array back to decimal (seq_t) */
    seq_t result = 0;
    for (int k = msd; k >= 0; --k) {
        result = result * 4 + e[k];
    }

    return result;
}
