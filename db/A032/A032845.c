/*
 * A032845  Numbers whose base-7 representation Sum_{i=0..m} d(i)*7^i has d(m)
 *          < d(m-1) > d(m-2) < ...
 * https://oeis.org/A032845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer whose base‑7 digits alternate
 *            < > < > ... starting with a < comparison between the
 *            most‑significant and the next digit. Single‑digit numbers
 *            automatically satisfy this
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool good(seq_t x)
{
    /* Single digit in base‑7 is always okay. */
    if (x < (seq_t)7)
        return true;

    /* Extract base‑7 digits (least significant first). */
    seq_t tmp = x;
    int d[32];      /* plenty of room for all practical values of n up to 10⁴ */
    int len = 0;
    while (tmp > (seq_t)0) {
        d[len++] = (int)(tmp % (seq_t)7);
        tmp /= (seq_t)7;
    }

    /* Check alternating inequalities from the most‑significant digit downwards.
       The first comparison (between the two most‑significant digits) must be <. */
    bool expect_less = true;
    for (int i = len - 1; i >= 1; --i) {
        int prev = d[i];
        int curr = d[i - 1];
        if (expect_less) {
            if (prev >= curr) return false;
        } else {
            if (prev <= curr) return false;
        }
        expect_less = !expect_less;
    }
    return true;
}

/* Return a(n) – 1‑indexed. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n < 1, harmless */

    idx_t count = 0;
    seq_t x = (seq_t)1;

    /* Scan positive integers until the n‑th valid one is found. */
    for (;; ++x) {
        if (good(x)) {
            ++count;
            if (count == n)
                return x;
        }
    }
}
