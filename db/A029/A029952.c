/*
 * A029952  Palindromic in base 5.
 * https://oeis.org/A029952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th nonnegative integer whose base‑5 representation
 *            is a palindrome
 * Range:     u8 n <= 35, u16 n <= 648, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset handling */
    idx_t count = 0;
    for (int len = 1; ; ++len) {
        if (len == 1) {
            for (int d = 0; d <= 4; ++d) {
                ++count;
                if (count == n) return (seq_t)d;
            }
        } else {
            int halfLen = (len + 1) / 2;
            /* leftHalf ranges from 5^(halfLen-1) to 5^halfLen - 1 */
            idx_t pow5 = 1;
            for (int k = 1; k < halfLen; ++k) pow5 *= 5;
            idx_t start = pow5;
            idx_t end   = pow5 * 5 - 1;
            int leftDigits[16];
            int fullDigits[32];
            for (idx_t leftHalf = start; leftHalf <= end; ++leftHalf) {
                idx_t tmp = leftHalf;
                for (int i = halfLen - 1; i >= 0; --i) {
                    leftDigits[i] = tmp % 5;
                    tmp /= 5;
                }
                for (int i = 0; i < halfLen; ++i) fullDigits[i] = leftDigits[i];
                for (int i = halfLen; i < len; ++i) fullDigits[i] = fullDigits[len - 1 - i];
                seq_t val = 0;
                for (int i = 0; i < len; ++i)
                    val = val * ((seq_t)5) + fullDigits[i];
                ++count;
                if (count == n) return val;
            }
        }
    }
    return 0; /* unreachable */
}
