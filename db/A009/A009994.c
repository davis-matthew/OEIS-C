/*
 * A009994  Numbers with digits in nondecreasing order.
 * https://oeis.org/A009994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = the nth nonnegative integer whose decimal digits are in
 *            nondecreasing order (OEIS A009994). The sequence is
 *            0,1,2,3,4,5,6,7, 8,9,11,12,13,14,15,16,17,18,19,22,23,24,25,26,
 * Range:     u8 n <= 122, u16 n <= 1946, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute binomial coefficient C(n,k) using only integer arithmetic.
   The calculation is performed in seq_t to keep the result within the
   available precision.  n and k are non‑negative and fit in idx_t. */
static seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - i + 1) / (seq_t)i;
    }
    return res;
}

/* Return the nth term of A009994 (1‑based index n ≥ 1).  The implementation
   determines the length d of the number, then selects the digits
   combinatorially.  All arithmetic uses only integer operations. */
seq_t a(idx_t n)
{
    /* Convert to 0‑based index for calculations. */
    seq_t k = (seq_t)(n - 1);

    /* Find the number of digits d.  For d = 1 there are 10 numbers
       (0‑9).  For d ≥ 2 the count is C(d+8,8). */
    idx_t d = 0;
    seq_t cumulative = 0;  /* count of all numbers with ≤ d digits */

    for (;;) {
        ++d;
        seq_t count;
        if (d == 1) {
            count = 10;                     /* digits 0‑9 */
        } else {
            count = binom_seq(d + 8, 8);    /* C(d+8,8) */
        }
        cumulative += count;
        if (k < cumulative) break;
    }

    /* Offset within numbers of length d.  Subtract the count of shorter
       lengths from the cumulative total. */
    seq_t prev_cumulative = cumulative;
    seq_t count;
    if (d == 1) {
        count = 10;
    } else {
        count = binom_seq(d + 8, 8);
    }
    prev_cumulative -= count;
    seq_t offset = k - prev_cumulative;

    /* Construct the number.  For d == 1 simply return offset (0‑9). */
    if (d == 1) {
        return offset;
    }

    /* For multi‑digit numbers digits are ≥ last chosen digit.
       The first digit cannot be 0, so its minimum is 1. */
    seq_t result = 0;
    int next_min = 1;  /* minimal allowed digit for the current position */

    for (idx_t pos = 0; pos < d; ++pos) {
        int chosen = -1;
        idx_t rem = d - pos - 1;   /* positions still to fill after this one */

        for (int digit = next_min; digit <= 9; ++digit) {
            /* Count how many numbers can be formed by filling the remaining
               rem positions with digits ≥ digit.  The available digits are
               from digit to 9 inclusive, a total of (10 - digit) choices. */
            seq_t ways = binom_seq(rem + (10 - digit) - 1, rem);
            if (offset >= ways) {
                offset -= ways;
            } else {
                chosen = digit;
                next_min = digit;  /* nondecreasing property */
                break;
            }
        }

        if (chosen == -1) chosen = 9;   /* safety: should never happen */
        result = result * 10 + (seq_t)chosen;
    }

    return result;
}
