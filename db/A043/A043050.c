/*
 * A043050  a(n)=(s(n)+4)/5, where s(n)=n-th base 5 palindrome that starts
 *          with 1.
 * https://oeis.org/A043050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+4)/5 where s(n) is the nth base‑5 palindrome that
 *            starts with the digit 1
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset starts at 1 */
    idx_t remaining = n;
    for (idx_t len = 1; ; ++len) {
        /* half length of the palindrome (ceil(len/2)) */
        idx_t half = (len + 1) / 2;
        /* number of palindromes of this length: 5^(half-1) */
        idx_t cnt = 1;
        for (idx_t i = 1; i < half; ++i) cnt *= 5;
        if (remaining <= cnt) {
            /* we need the (remaining)th palindrome of this length */
            idx_t rest = remaining - 1;          /* 0‑based index within this block */
            uint8_t digits[32];                  /* base‑5 digits of the half part */
            digits[0] = 1;                       /* first digit is always 1 */
            /* fill the remaining half‑1 digits (with leading zeros) */
            for (idx_t i = half - 1; i > 0; --i) {
                digits[i] = rest % 5;
                rest /= 5;
            }
            seq_t val = 0;
            /* first half of the palindrome */
            for (idx_t i = 0; i < half; ++i)
                val = val * 5 + digits[i];
            /* mirror for the second half (skip middle digit if odd length) */
            idx_t start = (len % 2 == 0) ? (half - 1) : (half - 2);
            for (idx_t i = start; (idx_t)i < len; --i) {
                val = val * 5 + digits[i];
            }
            /* compute a(n) = (s(n)+4)/5 */
            return (val + 4) / 5;
        }
        remaining -= cnt;  /* skip this block and continue with next length */
    }
}
