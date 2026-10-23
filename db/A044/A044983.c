/*
 * A044983  Numbers whose base-3 representation contains exactly three 0's and
 *          three 1's.
 * https://oeis.org/A044983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑3 representation contains exactly three 0's and three 1's. */
#include <stdint.h>
#include <stdbool.h>

/* Compute the binomial coefficient C(n,k) as a value of type seq_t. */
static seq_t binom_u64(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Compute the number of sequences of length m that contain exactly
   zeros "zeros" zeros and "ones" ones (the rest are 2's).  It equals
   C(m,zeros)·C(m-zeros,ones). */
static seq_t combine_possibilities(idx_t m, idx_t zeros, idx_t ones)
{
    return binom_u64(m, zeros) * binom_u64(m - zeros, ones);
}

/* External linkage function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)                /* behaviour below offset is irrelevant */
        return 0;

    seq_t sn = (seq_t)n;       /* convert index to seq_t for comparison   */
    seq_t cumulative = 0;      /* number of terms of all lengths < L     */

    /* The minimum length is 6: three 0's and three 1's, no 2's. */
    idx_t L = 6;

    /* Find the length L for which the n-th number lies. */
    for (;; ++L) {
        seq_t total   = combine_possibilities(L, 3, 3);          /* all ways */
        seq_t leading = combine_possibilities(L - 1, 2, 3);      /* leading 0 */
        seq_t count   = total - leading;                        /* valid numbers of this length */

        if (cumulative + count >= sn)
            break;
        cumulative += count;
    }

    /* Rank of the desired number among those of length L, 1‑based. */
    seq_t rem = sn - cumulative;

    /* Remaining digits to place. */
    idx_t zeros = 3;
    idx_t ones  = 3;
    idx_t twos  = (L > 6) ? (L - 6) : 0;

    seq_t val = 0;

    for (idx_t pos = 0; pos < L; ++pos) {
        for (int d = 0; d <= 2; ++d) {
            /* Constraints on the digit. */
            if (pos == 0 && d == 0)            /* no leading zeros          */
                continue;
            if (d == 0 && zeros == 0)
                continue;
            if (d == 1 && ones == 0)
                continue;
            if (d == 2 && twos == 0)
                continue;

            /* Count how many numbers remain if we choose digit d here. */
            idx_t nz = zeros, no = ones, nt = twos;
            if (d == 0)      nz--;
            else if (d == 1) no--;
            else             nt--;

            seq_t cnt_rest = combine_possibilities(L - pos - 1, nz, no);

            if (cnt_rest >= rem) {          /* digit d is chosen */
                val = val * 3 + (seq_t)d;
                if (d == 0)     zeros--;
                else if (d == 1) ones--;
                else             twos--;
                break;
            } else {                         /* skip this block of numbers */
                rem -= cnt_rest;
            }
        }
    }

    return val;
}
