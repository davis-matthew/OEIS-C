/*
 * A045189  Numbers whose base-5 representation contains no 0's and exactly
 *          two 3's.
 * https://oeis.org/A045189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth smallest integer whose base‑5 representation
 *            contains no 0's and exactly two 3's
 * Range:     u8 n <= 19, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   pow3_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include "../utils/pow3_int.h"

static seq_t binomial_int(int n, int k)
/* Return C(n,k) as seq_t.  Assumes 0 <= k <= n. */
{
    if (k < 0 || k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;
    seq_t r = (seq_t)1;
    for (int i = 1; i <= k; ++i)
        r = r * (n - k + i) / i;    /* division exact */
    return r;
}

/* ---- main sequence function ---- */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n < 1, but guard */
        return (seq_t)0;

    /* 1. Find the length L of the base‑5 representation that
       contains the nth number. */
    seq_t cum = (seq_t)0;      /* cumulative count of numbers seen so far */
    int L = 2;                 /* minimal length (two 3's need at least 2 digits) */

    for (;; ++L) {
        seq_t this_len = binomial_int(L, 2) * pow3_int(L - 2);
        seq_t cum_next = cum + this_len;
        if ((seq_t)n <= cum_next)          /* n is within the current length */
            break;
        cum = cum_next;                    /* advance to next length */
    }

    /* 2. Offset within numbers of length L. */
    seq_t idx = (seq_t)n - cum - (seq_t)1;  /* 0‑based index within this length */

    /* 3. Unrank the number digit by digit. */
    int digits[64];                      /* sufficient for all expected lengths */
    int used_threes = 0;

    for (int pos = 0; pos < L; ++pos) {
        int remaining_pos = L - pos - 1;

        /* Try digits in ascending order: 1,2,3,4 */
        for (int d = 1; d <= 4; ++d) {
            if (d == 3 && used_threes == 2)
                continue;               /* already used two 3's */

            int remaining_threes = 2 - used_threes - (d == 3 ? 1 : 0);
            if (remaining_threes < 0 || remaining_threes > remaining_pos)
                continue;               /* impossible to fulfill constraints */

            seq_t cnt = binomial_int(remaining_pos, remaining_threes)
                        * pow3_int(remaining_pos - remaining_threes);

            if (idx < cnt) {            /* the desired number has this digit */
                digits[pos] = d;
                used_threes += (d == 3 ? 1 : 0);
                break;
            } else {
                idx -= cnt;             /* skip all numbers with this prefix */
            }
        }
    }

    /* 4. Convert the base‑5 digit array to a seq_t value. */
    seq_t value = (seq_t)0;
    for (int pos = 0; pos < L; ++pos)
        value = value * (seq_t)5 + (seq_t)digits[pos];

    return value;
}
