/*
 * A029953  Palindromic in base 6.
 * https://oeis.org/A029953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth palindromic number in base 6 (a(1)=0)
 * Range:     u8 n <= 42, u16 n <= 519, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) is the first listed value, 0 */
    if (n == 1) return (seq_t)0;
    if (n == 0) return (seq_t)0;      /* safety for out‑of‑range */

    /* subtract the zero term */
    idx_t remaining = n - 1;

    for (int len = 1; ; ++len) {
        /* Count palindromes of this length */
        idx_t count;
        if (len == 1) {
            count = 5;         /* digits 1..5 */
        } else {
            int half = (len + 1) / 2;   /* ceil(len/2) */
            count = (idx_t)5;           /* first digit 1..5 */
            for (int i = 1; i < half; ++i)
                count *= 6;              /* remaining half‑1 digits 0..5 */
        }

        if (remaining > count) {
            remaining -= count;
            continue;
        }

        /* target palindromes are in this length */
        int half       = (len + 1) / 2;
        idx_t idx_pref = remaining - 1;          /* 0‑based among prefixes */
        uint8_t digits[10];                     /* half ≤ 5 for n ≤ 10000 */

        if (half == 1) {
            /* only the first digit varies (1..5) */
            digits[0] = (uint8_t)(idx_pref + 1);
        } else {
            /* mixed radix: first digit offset 0..4, others 0..5 */
            idx_t suffix_pow = 1;
            for (int i = 1; i < half; ++i) suffix_pow *= 6;

            idx_t first_off = idx_pref / suffix_pow;
            digits[0] = (uint8_t)(first_off + 1);

            idx_t rem = idx_pref % suffix_pow;
            for (int pos = half - 1; pos >= 1; --pos) {
                digits[pos] = (uint8_t)(rem % 6);
                rem /= 6;
            }
        }

        /* Build full digit array of the palindrome */
        uint8_t full[20];
        for (int i = 0; i < half; ++i) full[i] = digits[i];

        if (len % 2 == 0) {               /* even length */
            for (int i = 0; i < half; ++i)
                full[half + i] = digits[half - 1 - i];
        } else {                          /* odd length */
            for (int i = 0; i < half - 1; ++i)
                full[half + i] = digits[half - 2 - i];
        }

        /* Convert digit array to numeric value */
        seq_t val = 0;
        for (int i = 0; i < len; ++i)
            val = val * 6 + full[i];

        return val;
    }
}
