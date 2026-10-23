/*
 * A043311  a(n)=A033005(n)/8.
 * https://oeis.org/A043311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th positive integer whose base‑49 representation
 *            uses only digits 0–6 and has no two adjacent digits equal
 * Range:     u8 n <= 36, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;            /* convert index to seq_t */

    /* Find the length L of the base‑49 number.
       Count of numbers with length L is 6^L. */
    seq_t pow = 6;                 /* 6^1 */
    seq_t sum = 6;                 /* cumulative count up to current length */
    int L = 1;
    while (sum < N) {
        ++L;
        pow *= 6;                  /* 6^L */
        sum += pow;
    }

    seq_t prev_sum = sum - pow;    /* total up to length L-1 */
    seq_t offset   = N - prev_sum; /* 1‑based index within length L */
    seq_t k        = offset - 1;    /* 0‑based */

    /* Temporary storage for base‑6 digits and the final allowed digits. */
    static const int MAX_L = 64;
    uint8_t d[MAX_L];
    uint8_t digits[MAX_L];

    /* Convert k to base 6 with exactly L digits (most significant first). */
    seq_t divisor = pow / 6;       /* 6^(L-1) */
    for (int i = 0; i < L; ++i) {
        d[i] = (uint8_t)(k / divisor);
        k   %= divisor;
        if (i + 1 < L) divisor /= 6;
    }

    /* Map the base‑6 digits to valid base‑49 digits.
       First digit cannot be 0; subsequent digits skip the previous digit. */
    digits[0] = d[0] + 1;          /* 1–6 */
    for (int i = 1; i < L; ++i) {
        uint8_t prev = digits[i - 1];
        uint8_t val  = d[i];
        digits[i] = (val < prev) ? val : val + 1;
    }

    /* Construct the numeric value from the digits (base 49). */
    seq_t val = 0;
    for (int i = 0; i < L; ++i) {
        val = val * 49 + digits[i];
    }
    return val;
}
