/*
 * A044899  Base 13 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer >0 whose base‑13 digit run lengths alternate
 *            even, odd, even… starting with an even run
 * Range:     u8 n <= 24, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_good(seq_t x)
{
    if (x == 0)
        return false;

    /* store base‑13 digits (least significant first) */
    int digits[64];
    int len = 0;
    seq_t tmp = x;
    while (tmp) {
        digits[len++] = (int)(tmp % 13);
        tmp /= 13;
    }

    /* reverse to process from most significant digit */
    for (int i = 0; i < len / 2; ++i) {
        int t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    int expect_even = 1;   /* first run must have even length */
    int i = 0;
    while (i < len) {
        int d = digits[i];
        int run = 1;
        int j = i + 1;
        while (j < len && digits[j] == d) {
            ++run;
            ++j;
        }

        if (expect_even) {          /* need even length */
            if (run & 1)            /* odd length -> fail */
                return false;
        } else {                    /* need odd length */
            if (!(run & 1))         /* even length -> fail */
                return false;
        }

        expect_even ^= 1;           /* toggle expected parity */
        i = j;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n==0 */

    seq_t cur = 1;
    idx_t cnt = 0;

    while (1) {
        if (is_good(cur)) {
            ++cnt;
            if (cnt == n)
                return cur;
        }
        ++cur;
    }
}
