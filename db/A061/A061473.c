/*
 * A061473  First (leftmost) digit - second digit + third digit - fourth digit
 *          .... = 4.
 * https://oeis.org/A061473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 944 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061473: a(n) = the n‑th positive integer whose alternating
   digit sum (first digit + second digit – third digit + …, starting
   with a plus for the leftmost digit) equals 4. */

#include <stddef.h>

/* Compute the alternating digit sum of x, starting with + for the most
   significant digit. The result fits in a signed int. */
static int altsum(seq_t x)
{
    /* 256 decimal digits easily cover the growth needed for
       thousands of terms; if seq_t is truly arbitrary precision
       larger numbers may occur, but 256 is sufficient for our
       purposes. */
    unsigned char digits[256];
    int len = 0;

    while (x > (seq_t)0) {
        digits[len++] = (unsigned char)(x % (seq_t)10);
        x = x / (seq_t)10;
    }

    /* If x was 0 (not used for this sequence) return 0. */
    if (len == 0)
        return 0;

    int sum = 0;
    for (int i = 0; i < len; ++i) {
        int d = digits[len - 1 - i];   /* digit from most to least sig. */
        if (i % 2 == 0)   /* even index -> + */
            sum += d;
        else              /* odd index  -> - */
            sum -= d;
    }
    return sum;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cur = 1;

    while (1) {
        if (altsum(cur) == 4) {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
}
