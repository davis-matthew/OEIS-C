/*
 * A226797  Number of n-digit numbers x such that the digits of x^2 occur with
 *          frequency 1.
 * https://oeis.org/A226797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Computes a(n) = number of n‑digit integers x such that all
 *            digits of x² are distinct. For n ≥ 6 the square has ≥12 digits,
 *            so repetitions are unavoidable and a(n) = 0. For n ≤ 5 we
 *            brute‑force all n‑digit x to count the valid ones
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    if (n >= 6)              /* too many digits for a distinct square */
        return 0;

    seq_t count = 0;                     /* result accumulator */

    /* compute bounds for n‑digit numbers */
    idx_t start, end;
    if (n == 1) {                /* 1‑digit numbers include 0 … 9 */
        start = 0;
        end   = 9;
    } else {
        idx_t power = 1;
        for (idx_t i = 1; i < n; ++i)
            power *= 10;       /* power = 10^(n-1) */
        start = power;
        end   = power * 10 - 1; /* end = 10^n - 1 */
    }

    /* brute‑force all candidates */
    for (idx_t x = start; x <= end; ++x) {
        seq_t sq = (seq_t)x * (seq_t)x;   /* square of x */

        unsigned char cnt[10] = {0};
        digit_counts(sq, cnt);           /* count digit frequencies */

        bool unique = true;
        for (int d = 0; d < 10 && unique; ++d)
            if (cnt[d] > 1)
                unique = false;

        if (unique)
            count += 1;
    }

    return count;
}
