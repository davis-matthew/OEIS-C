/*
 * A050432  Length of longest palindromic subword of (n base 4).
 * https://oeis.org/A050432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = length of the longest palindromic substring of the base‑4 representation of n */
seq_t a(idx_t n)
{
    if (n == 0)               /* not an official term, but keep safe */
        return (seq_t)0;

    /* convert n to base 4, store digits most‑significant first */
    int digits[32];          /* 4^15 > 10^9, plenty of room for idx_t up to 64 bits */
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (int)(tmp % 4);   /* least‑significant digit first */
        tmp /= 4;
    }
    /* reverse to most‑significant first */
    for (int i = 0; i < len / 2; ++i) {
        int t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    seq_t best = (seq_t)1;             /* at least one digit forms a palindrome */

    /* odd‑length centers */
    for (int center = 0; center < len; ++center) {
        int l = center, r = center;
        while (l >= 0 && r < len && digits[l] == digits[r]) {
            seq_t cur = (seq_t)(r - l + 1);
            if (cur > best) best = cur;
            --l; ++r;
        }
    }

    /* even‑length centers */
    for (int center = 0; center < len - 1; ++center) {
        int l = center, r = center + 1;
        while (l >= 0 && r < len && digits[l] == digits[r]) {
            seq_t cur = (seq_t)(r - l + 1);
            if (cur > best) best = cur;
            --l; ++r;
        }
    }

    return best;
}
