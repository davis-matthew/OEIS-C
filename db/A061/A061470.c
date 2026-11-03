/*
 * A061470  First (leftmost) digit - second digit + third digit - fourth digit
 *          .... = 1.
 * https://oeis.org/A061470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 3959, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 7773 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th positive integer whose alternating sum of
 * decimal digits (leftmost minus next plus next ...) equals 1.
 * The implementation simply scans successive integers and returns the
 * n-th one that satisfies this condition.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute whether the alternating digit sum of x equals 1. */
static inline bool alt_sum_is_one(seq_t x)
{
    int digits[32];    /* enough for 128‑bit numbers */
    int len = 0;

    /* Extract digits least‑significant first. */
    while (x > 0) {
        digits[len++] = (int)(x % 10);
        x /= 10;
    }

    /* Empty number never occurs (we start from 1). */
    if (len == 0)
        return false;

    int sum = 0;
    bool add = true;   /* leftmost digit is added */

    /* Process digits from most significant to least significant. */
    for (int i = len - 1; i >= 0; --i) {
        if (add)
            sum += digits[i];
        else
            sum -= digits[i];
        add = !add;
    }
    return sum == 1;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cur = 1;

    while (1) {
        if (alt_sum_is_one(cur)) {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
}
