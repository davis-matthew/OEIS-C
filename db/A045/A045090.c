/*
 * A045090  Numbers whose base-4 representation contains exactly one 1 and no
 *          2's.
 * https://oeis.org/A045090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose base-4 representation contains exactly one 1 and
 *            no 2's
 * Range:     u8 n <= 32, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find the length L of the desired term. */
    idx_t cum = 0;      /* cumulative count of terms up to previous length */
    idx_t L = 1;        /* current length (number of base-4 digits) */
    idx_t countL;

    while (1) {
        if (L == 1) {
            countL = 1;                       /* only "1" */
        } else {
            /* count of valid numbers of length L:
               (L+1) * 2^(L-2)  (derived from the combinatorics above) */
            countL = (idx_t)(L + 1) * ((idx_t)1 << (L - 2));
        }
        if (cum + countL >= n) {
            break;                            /* n lies in this length */
        }
        cum += countL;
        ++L;
    }

    /* Index k (0‑based) within the block of numbers of length L. */
    idx_t k = n - cum - 1;

    seq_t value = 0;
    bool used_one = false;                    /* whether the single '1' has been placed */

    /* Unrank the k-th pattern for length L. */
    for (idx_t pos = L; pos-- > 0; ) {         /* pos runs from L-1 down to 0 */
        int digit;                             /* chosen digit at this position */
        for (int di = 0; di < 3; ++di) {        /* try digits 0, 1, 3 in order */
            int d = (di == 0) ? 0 :
                    (di == 1) ? 1 : 3;

            /* MSB cannot be 0; only one digit 1 allowed. */
            if (pos == L - 1 && d == 0)    continue;
            if (d == 1 && used_one)       continue;

            /* Count completions of the remaining positions. */
            idx_t r = pos;                  /* number of positions after this one */
            bool usedAfter = used_one || (d == 1);
            idx_t cnt;

            if (usedAfter) {
                /* All remaining digits are 0 or 3. */
                cnt = (idx_t)1 << r;
            } else {
                /* Need to place exactly one '1' in the remaining r positions. */
                if (r == 0) {
                    cnt = 0;                /* impossible to place a 1 */
                } else {
                    cnt = r * ((idx_t)1 << (r - 1));
                }
            }

            if (k < cnt) {                  /* this digit reaches the desired rank */
                digit = d;
                used_one = usedAfter;
                break;
            } else {
                k -= cnt;                    /* skip all numbers starting with this d */
            }
        }

        /* Append the chosen digit to the accumulating value. */
        value = value * (seq_t)4 + (seq_t)digit;
    }

    return value;
}
