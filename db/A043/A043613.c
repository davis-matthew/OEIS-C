/*
 * A043613  Numbers whose base-6 representation has exactly 5 runs.
 * https://oeis.org/A043613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth smallest integer whose base-6 representation has
 *            exactly 5 runs
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation un‐ranks the number by combinatorial enumeration of runs. */

#include <stdint.h>
#include <stdbool.h>

/* Helper: binomial coefficient C(n,k) for small n (n <= 20). */
static seq_t binom_seq_t(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* Pre‑computed powers: 5^i for i = 0..5. */
static const seq_t pow5[6] = {1, 5, 25, 125, 625, 3125};

seq_t a(idx_t n)
{
    /* Find the base‑6 length L such that the nth term lies in the block of length L. */
    seq_t cum = 0;
    idx_t L = 5;
    while (1)
    {
        seq_t cnt = binom_seq_t(L - 1, 4) * pow5[5];   /* C(L-1,4)*5^5 */
        if (cum + cnt >= (seq_t)n) break;
        cum += cnt;
        ++L;
    }

    seq_t k = (seq_t)n - cum - 1;   /* zero‑based index within length‑L group */

    seq_t value = 0;
    int prev = -1;          /* previous digit (-1 denotes none yet) */
    idx_t runs_done = 0;    /* how many runs have been started so far */

    for (idx_t pos = 0; pos < L; ++pos)
    {
        int min_d = (pos == 0) ? 1 : 0;
        for (int d = min_d; d <= 5; ++d)
        {
            idx_t r_new = runs_done;
            if (pos == 0)
                r_new = 1;
            else if (d != prev)
                r_new = runs_done + 1;
            /* If this choice would create more than 5 runs, skip it. */
            if (r_new > 5) continue;

            idx_t rem = L - pos - 1;          /* remaining positions after this one */
            seq_t completions;
            if (r_new == 5)
            {
                /* All remaining digits must equal d.  Exactly one completion. */
                completions = 1;
            }
            else
            {
                idx_t remaining_runs = 5 - r_new;
                if (rem < remaining_runs)
                    completions = 0;
                else
                    completions = binom_seq_t(rem - 1, remaining_runs - 1) * pow5[remaining_runs];
            }

            if (k < completions)
            {
                /* Commit this digit. */
                value = value * 6 + (seq_t)d;
                prev = d;
                runs_done = r_new;
                break;
            }
            else
            {
                k -= completions;
            }
        }
    }

    return value;
}
