/*
 * A011001  Binomial coefficient C(n,48).
 * https://oeis.org/A011001
 *
 * Offset:    48    (a(48) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n,48) (binomial coefficient)
 * Range:     u8 n <= 49, u16 n <= 51, u32 n <= 56, u64 n <= 72, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t _gcd_idx(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n < 48) return (seq_t)0;          /* undefined for n<48 */

    /* numerators of the binomial coefficient: n-48+1 … n */
    seq_t nums[48];
    for (idx_t i = 0; i < 48; ++i) {
        nums[i] = (seq_t)(n - 48 + i + 1);
    }

    /* cancel all factors of the denominator 48! from the numerators  */
    for (idx_t d = 2; d <= 48; ++d) {
        idx_t denom = d;
        for (idx_t i = 0; i < 48 && denom > 1; ++i) {
            idx_t g = _gcd_idx((idx_t)nums[i], denom);
            if (g > 1) {
                nums[i] /= g;
                denom /= g;
            }
        }
    }

    /* multiply the remaining numerators – no overflow will occur
       because the product never exceeds the final result */
    seq_t res = 1;
    for (idx_t i = 0; i < 48; ++i) {
        res *= nums[i];
    }
    return res;
}
