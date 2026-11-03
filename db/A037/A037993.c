/*
 * A037993  Numbers whose maximal base-8 run length is 2.
 * https://oeis.org/A037993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9803 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A037993: Numbers whose maximal base-8 run length is 2 (search) */
#include <stdbool.h>
#include <stddef.h>

/* helper function: true if x has a run of length 2 in base 8 and no run longer than 2 */
static bool has_valid_oct_run(seq_t x)
{
    const seq_t eight = 8;
    int prev_digit = -1;
    int run_len = 0;
    bool has_pair = false;

    while (x > 0) {
        seq_t d_seq = x % eight;
        int digit = (int)d_seq;
        if (digit == prev_digit) {
            run_len++;
        } else {
            run_len = 1;
            prev_digit = digit;
        }
        if (run_len > 2) return false;
        if (run_len == 2) has_pair = true;
        x = x / eight;
    }
    return has_pair;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* behavior below offset is undefined */
    idx_t count = 0;
    seq_t cand = 1;                     /* start from 1 (0 never satisfies) */

    for (;; ++cand) {
        if (has_valid_oct_run(cand)) {
            count++;
            if (count == n) return cand;
        }
    }
}
