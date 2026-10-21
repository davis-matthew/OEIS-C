/*
 * A211096  Smallest (i.e., rightmost) Lyndon word in the Lyndon factorization
 *          of the binary representation of n (written using 1's and 2's
 *          rather than 0's and 1's, since numbers > 0 in the OEIS cannot
 *          begin with 0).
 * https://oeis.org/A211096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rightmost Lyndon factor of the binary representation of
 *            n, with 0→1 and 1→2
 * Range:     u8 n <= 16, u16 n <= 64, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Build binary representation of n into s[0..len-1] */
    char s[65];                 /* enough for 64‑bit integers */
    int len;

    if (n == 0) {               /* special‑case */
        s[0] = '0';
        len = 1;
    } else {
        len = 0;
        idx_t tmp = n;
        /* write bits in reverse order */
        while (tmp) {
            s[len++] = (tmp & 1) ? '1' : '0';
            tmp >>= 1;
        }
        /* reverse to correct order */
        for (int i = 0; i < len / 2; ++i) {
            char t = s[i];
            s[i] = s[len - 1 - i];
            s[len - 1 - i] = t;
        }
    }

    /* Duval's algorithm for Lyndon factorization: keep only the length of the
       last factor. */
    int i = 0, last_len = 0;
    while (i < len) {
        int j = i + 1, k = i;
        while (j < len && s[k] <= s[j]) {
            if (s[k] < s[j])   k = i;
            else               ++k;
            ++j;
        }
        int step = j - k;
        while (i <= k) {
            last_len = step;
            i += step;
        }
    }

    /* Convert the last factor to a decimal number using digits 1 and 2. */
    idx_t start = (idx_t)(len - last_len);
    seq_t val = 0;
    for (int p = start; p < len; ++p) {
        int d = (s[p] == '0') ? 1 : 2;   /* 0 → 1, 1 → 2 */
        val = val * (seq_t)10 + (seq_t)d;
    }
    return val;
}
