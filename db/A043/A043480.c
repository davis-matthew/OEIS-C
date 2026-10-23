/*
 * A043480  Numbers having four 6's in base 9.
 * https://oeis.org/A043480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑9 representation contains exactly four digit
 *            6
 * Range:     u8 none, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t binom_idx(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t r = 1;
    for (idx_t i = 1; i <= k; ++i)
        r = (r * (n - k + i)) / i;
    return r;
}

/* powers of 8 (up to 8^10, more than enough for this sequence) */
static const idx_t pow8_arr[11] = {
    1ULL,                 /* 8^0 */
    8ULL,                 /* 8^1 */
    64ULL,                /* 8^2 */
    512ULL,               /* 8^3 */
    4096ULL,              /* 8^4 */
    32768ULL,             /* 8^5 */
    262144ULL,            /* 8^6 */
    2097152ULL,           /* 8^7 */
    16777216ULL,          /* 8^8 */
    134217728ULL,         /* 8^9 */
    1073741824ULL         /* 8^10 */
};

static idx_t count_remaining(idx_t rem, idx_t sixes_needed) {
    if (sixes_needed > rem) return 0;
    return binom_idx(rem, sixes_needed) * pow8_arr[rem - sixes_needed];
}

static idx_t count_len(idx_t L) {
    if (L < 4) return 0;
    idx_t cnt1 = 0, cnt2 = 0;
    /* leading digit = 6 */
    if (L >= 4 && L - 1 >= 3)
        cnt1 = binom_idx(L - 1, 3) * pow8_arr[L - 4];
    /* leading digit ≠ 0,6 */
    if (L >= 5 && L - 1 >= 4)
        cnt2 = 7 * binom_idx(L - 1, 4) * pow8_arr[L - 5];
    return cnt1 + cnt2;
}

seq_t a(idx_t n) {
    if (n < 1) return 0; /* undefined for n < 1, return 0 */

    /* Find the length L of the base‑9 representation of the n‑th element */
    idx_t L = 4;
    idx_t cumulative = 0;
    while (cumulative + count_len(L) < n) {
        cumulative += count_len(L);
        ++L;
    }

    idx_t m = n - cumulative - 1;          /* 0‑based index within length L block */
    idx_t remaining_sixes = 4;
    uint8_t digits[32];                     /* 32 digits is far more than enough */

    for (idx_t pos = 0; pos < L; ++pos) {
        int start = (pos == 0) ? 1 : 0;
        int chosen = -1;
        idx_t rem = L - 1 - pos;

        for (int d = start; d <= 8; ++d) {
            idx_t need = remaining_sixes - ((d == 6) ? 1 : 0);
            idx_t cnt = count_remaining(rem, need);
            if (m < cnt) {
                chosen = d;
                remaining_sixes = need;
                break;
            }
            m -= cnt;
        }
        /* chosen must be valid because a valid number exists */
        digits[pos] = (uint8_t)chosen;
    }

    /* Convert base‑9 digits to decimal (seq_t) */
    seq_t value = 0;
    for (idx_t i = 0; i < L; ++i)
        value = value * 9 + digits[i];

    return value;
}
