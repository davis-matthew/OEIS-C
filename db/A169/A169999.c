/*
 * A169999  Carryless product n X n in base 3.
 * https://oeis.org/A169999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = carryless product of n by itself in base 3, * computed by
 *            squaring the base‑3 representation of n, summing * the ordinary
 *            partial products, reducing each coefficient modulo 3, * and
 *            interpreting the resulting digits as a base‑3 number
 * Range:     u8 n <= 26, u16 n <= 269, u32 n <= 9871, u64 n <= 9871,
 *            u128 n <= 9871, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert n to base‑3 digits (least significant first). */
    idx_t d = 0;                     /* number of digits */
    idx_t tmp = n;
    unsigned char digits[64] = {0};   /* max digits for 64‑bit n ≈ 41   */
    while (tmp > 0) {
        digits[d++] = (unsigned char)(tmp % 3);
        tmp /= 3;
    }

    /* Convolution: compute coefficient array for n × n (no carry). */
    idx_t max_coeff = 2 * d;          /* length needed for convolution */
    seq_t coeff[128] = {0};           /* 128 ≥ 2*41 safely covers all n */
    for (idx_t i = 0; i < d; ++i) {
        for (idx_t j = 0; j < d; ++j) {
            coeff[i + j] += (seq_t)digits[i] * (seq_t)digits[j];
        }
    }

    /* Reduce each coefficient modulo 3 (carryless). */
    for (idx_t t = 0; t < max_coeff; ++t) {
        coeff[t] %= 3;
    }

    /* Interpret result as a base‑3 number and convert to decimal. */
    seq_t res = 0;
    for (idx_t t = max_coeff; t > 0; --t) {
        res = res * 3 + coeff[t - 1];
    }
    return res;
}
