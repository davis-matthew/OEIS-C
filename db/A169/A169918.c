/*
 * A169918  Squares in carryless arithmetic mod 10 with addition of digits
 *          defined to be multiplication mod 10 and multiplication of digits
 *          defined to be addition mod 10.
 * https://oeis.org/A169918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = square of n in carryless arithmetic where digit addition
 *            = multiplication mod 10 and digit multiplication = addition mod
 *            10
 * Range:     u8 n <= 11, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Determine decimal digits of n (least significant first). */
    unsigned int len = 0;
    unsigned char digits[64];  /* 64 digits are enough for 64‑bit integers. */

    if (n == 0) {
        len = 1;
        digits[0] = 0;
    } else {
        idx_t temp = n;
        while (temp > 0) {
            digits[len++] = (unsigned char)(temp % 10);
            temp /= 10;
        }
    }

    /* Result can have at most 2*len-1 digits. */
    unsigned int res_len = 2 * len - 1;
    unsigned char res[128];           /* 128 is safely more than 2*64-1. */
    for (unsigned int i = 0; i < res_len; ++i) {
        res[i] = 1;                   /* multiplicative identity. */
    }

    /* Compute the carryless product (square) digit by digit. */
    for (unsigned int i = 0; i < len; ++i) {
        for (unsigned int j = 0; j < len; ++j) {
            unsigned char contrib = (digits[i] + digits[j]) % 10;
            res[i + j] = (unsigned char)((res[i + j] * contrib) % 10);
        }
    }

    /* Assemble the decimal number from the result digits. */
    seq_t out = 0;
    for (int k = (int)res_len - 1; k >= 0; --k) {
        out = out * 10 + res[k];
    }
    return out;
}
