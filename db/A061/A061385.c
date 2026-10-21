/*
 * A061385  Numbers n such that sum of digits = twice number of digits.
 * https://oeis.org/A061385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 826, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A061385:  Numbers n such that sum of digits = twice number of digits. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* The build system supplies the types seq_t (output type) and idx_t
   (index type). */
extern seq_t a(idx_t n);   /* Declared only for the prototypes. */

#define MAXK 30               /* enough for the first 10 000 terms in every build */
#define MAXSUM (9 * MAXK)

/* Count of ways to write a sum with a given number of digits.  Since the
   number of ways can be much larger than a byte, we use idx_t internally. */
static bool initialized = false;
static idx_t ways[MAXK + 1][MAXSUM + 1];
static idx_t cum[MAXK + 1];

static void init_dp(void)
{
    /* ways[0][0] = 1, others 0 */
    for (int m = 0; m <= MAXK; ++m)
        for (int s = 0; s <= MAXSUM; ++s)
            ways[m][s] = 0;
    ways[0][0] = 1;

    /* DP for digit sums of length m */
    for (int m = 1; m <= MAXK; ++m) {
        for (int s = 0; s <= 9 * m; ++s) {
            idx_t sum = 0;
            for (int d = 0; d <= 9; ++d) {
                if (s >= d)
                    sum += ways[m - 1][s - d];
            }
            ways[m][s] = sum;
        }
    }

    /* cumulative counts of valid numbers per digit length */
    cum[0] = 0;
    for (int k = 1; k <= MAXK; ++k) {
        idx_t cnt = 0;
        for (int first = 1; first <= 9; ++first) {
            int rem = 2 * k - first;          /* remaining sum for the other digits */
            if (rem < 0 || rem > 9 * (k - 1))
                continue;
            cnt += ways[k - 1][rem];
        }
        cum[k] = cum[k - 1] + cnt;
    }

    initialized = true;
}

seq_t a(idx_t n)          /* n >= 1 */
{
    if (!initialized)
        init_dp();

    /* find the number of digits k so that cum[k-1] < n <= cum[k] */
    int k = 1;
    while (k <= MAXK && cum[k] < n)
        ++k;
    if (k > MAXK)   /* safety, should not happen for asked range */
        return 0;

    /* rank of the term within the k-digit block (starting from 1) */
    idx_t rank = n - cum[k - 1];

    /* unrank: build the number digit by digit */
    seq_t result = 0;
    int digits_left = k;
    int sum_left = 2 * k;

    for (int pos = 1; pos <= k; ++pos) {
        int start = (pos == 1) ? 1 : 0;
        for (int d = start; d <= 9; ++d) {
            if (sum_left < d)
                continue;
            int rem = sum_left - d;
            int remaining_digits = digits_left - 1;
            if (rem > 9 * remaining_digits)
                continue;

            idx_t avail = ways[remaining_digits][rem];
            if (rank > avail) {
                rank -= avail;
            } else {
                result = result * (seq_t)10 + (seq_t)d;
                sum_left -= d;
                digits_left -= 1;
                break;
            }
        }
    }

    return result;
}
