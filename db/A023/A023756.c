/*
 * A023756  Plaindromes: numbers whose digits in base 15 are in nondecreasing
 *          order.
 * https://oeis.org/A023756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th nonnegative integer whose base‑15 digits are in
 *            nondecreasing order (allowing equal digits). a(1)=0, a(2)=1, …
 * Range:     u8 n <= 134, u16 n <= 4560, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static inline size_t small_binom(size_t n, size_t k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    size_t r = 1;
    for (size_t i = 1; i <= k; ++i) {
        r = r * (n - k + i) / i;
    }
    return r;
}

static inline size_t count_len(size_t len) {
    if (len == 1) return 15;               /* digits 0‑14 */
    return small_binom(13 + len, len);     /* choose len from 1‑14 with repetition */
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                  /* offset starts at 1, 0 value irrelevant */
    size_t N = (size_t)n;

    /* Find the length of the representation that contains the n-th number. */
    size_t len = 1;
    size_t cum = 0;
    while (1) {
        size_t c = count_len(len);
        if (cum + c >= N) break;
        cum += c;
        ++len;
    }

    /* Rank within numbers of this length (0‑based). */
    size_t rank = N - cum - 1;

    if (len == 1) {
        return (seq_t)rank;                /* single digit case */
    }

    /* Unrank the nondecreasing sequence of digits. */
    int digits[16];                       /* maximum length needed for n ≤ 10000 is 5 */
    int prev = 1;                         /* leading digit ≥ 1 for len > 1 */
    size_t rem = rank;
    for (size_t pos = 0; pos < len; ++pos) {
        for (int d = prev; d <= 14; ++d) {
            size_t rest = len - pos - 1;
            size_t cnt;
            if (rest == 0) {
                cnt = 1;                  /* only this single digit */
            } else {
                /* Count sequences of length `rest` from digits d…14 */
                cnt = small_binom((14 - d) + rest, rest);
            }
            if (rem < cnt) {
                digits[pos] = d;
                prev = d;
                break;
            } else {
                rem -= cnt;
            }
        }
    }

    /* Convert the digit array to a decimal value. */
    seq_t value = 0;
    for (size_t i = 0; i < len; ++i) {
        value = value * 15 + (seq_t)digits[i];
    }
    return value;
}
