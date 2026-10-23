/*
 * A032977  Numbers with the property that all pairs of consecutive base-6
 *          digits differ by 0 or 1.
 * https://oeis.org/A032977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * This function returns the n‑th number whose base‑6 digits differ by at most
 * 1 in adjacent positions (OEIS A032977).  The implementation uses a
 * dynamic‑programming table that counts the number of valid suffixes for
 * each possible previous digit.  The nth term is obtained by determining
 * its length, then selecting the appropriate digits lexicographically.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Dynamic‑programming table.
 * dp[rem * 6 + prev] is the number of sequences of length 'rem'
 * where the first digit after the previous digit 'prev' must be chosen.
 * rem = number of digits still to place (including the first to be chosen),
 * prev = last digit placed (0–5).  A value of prev is never -1 when dp is read.
 */
static seq_t *dp = NULL;
static size_t dp_cap = 0;   /* largest remainder value that has been computed */

/* Ensure that dp contains data up to rem_needed inclusive.
 * After the call, dp[(rem * 6) + prev] is valid for all rem <= rem_needed
 * and 0 <= prev < 6.  dp[0][prev] is initialised to 1 for all prev. */
static void ensure_dp_size(size_t rem_needed)
{
    if (!dp || dp_cap < rem_needed) {
        /* Allocate space for (rem_needed + 1) rows (rem = 0 .. rem_needed) */
        size_t new_total = (rem_needed + 1) * 6;
        seq_t *new_dp;
        if (!dp) {
            new_dp = (seq_t *)malloc(new_total * sizeof(seq_t));
        } else {
            new_dp = (seq_t *)realloc(dp, new_total * sizeof(seq_t));
        }
        if (!new_dp) abort();  /* allocation failed */
        dp = new_dp;

        /* Initialise base row (rem = 0) */
        for (int d = 0; d < 6; ++d) dp[d] = 1;

        /* Compute new rows from old_cap+1 up to rem_needed */
        for (size_t r = dp_cap + 1; r <= rem_needed; ++r) {
            for (int prev = 0; prev < 6; ++prev) {
                seq_t sum = 0;
                for (int nxt = prev - 1; nxt <= prev + 1; ++nxt) {
                    if (nxt >= 0 && nxt < 6) sum += dp[(r - 1) * 6 + nxt];
                }
                dp[r * 6 + prev] = sum;
            }
        }
        dp_cap = rem_needed;
    }
}

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* outside defined offset, safe fallback */

    seq_t cumulative = 0;   /* count of all numbers with fewer digits than L */
    size_t L = 1;           /* current number of digits under consideration */

    /* 1. Find the digit length L that contains the n-th term. */
    while (true) {
        ensure_dp_size(L - 1);          /* need counts for (L-1) remaining digits */

        seq_t count_len = 0;
        for (int d = 1; d <= 5; ++d)    /* first digit cannot be 0 */
            count_len += dp[(L - 1) * 6 + d];

        seq_t new_cum = cumulative + count_len;
        if (new_cum >= (seq_t)n)        /* the desired term lies at length L */
            break;

        cumulative = new_cum;
        ++L;
    }

    /* 2. Determine the zero‑based index k within numbers of length L. */
    seq_t diff = (seq_t)n - cumulative - 1;
    idx_t k = (idx_t)diff;              /* safe because diff >= 0 here */

    /* 3. Build the decimal value digit by digit in base‑6. */
    seq_t value = 0;
    size_t remain = L;      /* digits still to place */
    int prev = -1;          /* no previous digit yet */

    /* 3a. Choose the first digit. */
    for (int d = 1; d <= 5; ++d) {
        seq_t count = dp[(L - 1) * 6 + d];
        if ((seq_t)k < count) {
            value = d;
            remain--;
            prev = d;
            break;
        } else {
            k -= (idx_t)count;
        }
    }

    /* 3b. Choose the remaining digits. */
    while (remain > 0) {
        for (int d = prev - 1; d <= prev + 1; ++d) {
            if (d < 0 || d > 5) continue;
            seq_t count = dp[(remain - 1) * 6 + d];
            if ((seq_t)k < count) {
                value = value * 6 + d;
                remain--;
                prev = d;
                break;
            } else {
                k -= (idx_t)count;
            }
        }
    }

    return value;
}
