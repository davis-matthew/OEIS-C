/*
 * A044854  Positive integers having no fewer base-13 runs of even length than
 *          odd.
 * https://oeis.org/A044854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044854: Positive integers having no fewer base-13 runs of even length than odd.
   Computation is performed by a brute‑force search. */
#include <stdbool.h>

static bool has_enough_even_runs(seq_t n)
{
    seq_t temp = n;
    int prev_digit = -1;
    int run_len   = 0;
    int even_cnt  = 0;
    int odd_cnt   = 0;

    while (temp > 0) {
        int digit = (int)(temp % (seq_t)13);
        temp     /= (seq_t)13;

        if (prev_digit == -1) {
            /* first digit starts the first run */
            prev_digit = digit;
            run_len    = 1;
            continue;
        }

        if (digit == prev_digit) {
            run_len++;
        } else {
            /* finish previous run */
            if ((run_len & 1) == 0)
                even_cnt++;
            else
                odd_cnt++;
            /* start new run */
            prev_digit = digit;
            run_len    = 1;
        }
    }

    /* account for the last run */
    if (run_len > 0) {
        if ((run_len & 1) == 0)
            even_cnt++;
        else
            odd_cnt++;
    }

    return even_cnt >= odd_cnt;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cand  = 1;

    while (1) {
        if (has_enough_even_runs(cand)) {
            count++;
            if (count == n)
                return cand;
        }
        cand++;
    }
}
