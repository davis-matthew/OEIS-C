/*
 * A194458  Total number of entries in rows 0,1,...,n of Pascal's triangle not
 *          divisible by 5.
 * https://oeis.org/A194458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{t=0}^n ∏_{i} (digit_i(t)+1), where digit_i(t) are
 *            the base‑5 digits of t
 * Range:     u8 n <= 29, u16 n <= 846, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the total number of entries in rows 0..n of Pascal's triangle not divisible by 5. */
seq_t a(idx_t n) {
    /* m = n + 1; we sum over t = 0 .. m-1 */
    idx_t m = n + 1;

    /* Convert m to base 5 digits (least significant first). */
    uint8_t digits[64];   /* 5^64 >> 2^256 > large */
    int len = 0;
    if (m == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (m > 0 && len < (int)sizeof(digits)) {
            digits[len++] = (uint8_t)(m % 5);
            m /= 5;
        }
    }

    /* Pre‑compute powers of 15 up to length len */
    seq_t pow15[65];
    pow15[0] = (seq_t)1;
    for (int i = 1; i <= len; ++i) {
        pow15[i] = pow15[i-1] * (seq_t)15;
    }

    seq_t result = (seq_t)0;
    seq_t prefix_prod = (seq_t)1;

    /* Process digits from most significant to least significant */
    for (int pos = len - 1; pos >= 0; --pos) {
        uint8_t dcur = digits[pos];
        int rem = pos;           /* number of lower positions */
        /* Numbers with current digit less than dcur */
        for (int d = 0; d < dcur; ++d) {
            seq_t add = prefix_prod * (seq_t)(d + 1) * pow15[rem];
            result += add;
        }
        /* Include the current digit equal to dcur for the prefix */
        prefix_prod *= (seq_t)(dcur + 1);
    }

    return result;
}
