/*
 * A061477  First (leftmost) digit - second digit + third digit - fourth digit
 *          .... = 8.
 * https://oeis.org/A061477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose alternating sum of
 *            decimal digits (leftmost digit added, next subtracted, next
 *            added, …) equals 8
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 769 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute alternating sum of decimal digits for an unsigned long long value.
   The leftmost digit has + sign, the next has – sign, etc. */
static inline int alt_sum_ull(unsigned long long x)
{
    unsigned int digits[20];   /* enough for 64‑bit values */
    int len = 0;
    unsigned long long t = x;

    if (t == 0) return 0;     /* not used for x == 0 */

    while (t > 0) {
        digits[len++] = (unsigned int)(t % 10);
        t /= 10;
    }

    int sum = 0;
    for (int i = 0; i < len; ++i) {
        int leftpos = len - 1 - i;                /* 0 for leftmost */
        int sign = (leftpos & 1) ? -1 : 1;        /* even -> +, odd -> – */
        sum += sign * (int)digits[i];
    }
    return sum;
}

/* primary sequence routine */
seq_t a(idx_t n)
{
    unsigned long long cand = 1;   /* iterate over natural numbers */
    idx_t found = 0;

    for (;;) {
        if (alt_sum_ull(cand) == 8) {
            ++found;
            if (found == n) {
                return (seq_t)cand;
            }
        }
        ++cand;   /* candidate fits in 64 bits for the required n ranges */
    }
}
