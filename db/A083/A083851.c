/*
 * A083851  Decimal palindromes that are not multiples of 11.
 * https://oeis.org/A083851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 596, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate increasing odd‑length decimal palindromes
   and return those that are not divisible by 11. */

seq_t a(idx_t n) {
    idx_t remaining = n;
    seq_t val;

    for (int len = 1; ; len += 2) {           /* odd lengths only */
        int half = (len + 1) / 2;              /* digits in the prefix   */
        seq_t start = 1;
        for (int i = 1; i < half; ++i) start *= 10;   /* 10^(half-1) */
        seq_t end = 1;
        for (int i = 0; i < half; ++i) end *= 10;    /* 10^half   */
        end -= 1;                                /* last prefix   */
        seq_t pow10k = 1;                         /* 10^(half-1) for mirroring */
        for (int i = 0; i < half - 1; ++i) pow10k *= 10;

        for (seq_t p = start; p <= end; ++p) {
            /* build palindrome from prefix p */
            seq_t tmp = p / 10;   /* drop middle digit */
            seq_t rev = 0;
            while (tmp > 0) {
                rev = rev * 10 + tmp % 10;
                tmp /= 10;
            }
            val = p * pow10k + rev;    /* full odd‑length palindrome */

            if ((val % 11) != 0) {        /* skip multiples of 11 */
                if (--remaining == 0)
                    return val;
            }
        }
    }
}
