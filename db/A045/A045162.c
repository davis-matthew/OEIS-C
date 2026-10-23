/*
 * A045162  Numbers whose base-5 representation contains exactly one 0 and one
 *          1.
 * https://oeis.org/A045162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑5 representation
 *            contains exactly one 0 and exactly one 1
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


static inline idx_t power3(idx_t e)
{
    idx_t res = 1;
    for (idx_t i = 0; i < e; ++i)
        res *= 3;
    return res;
}

seq_t a(idx_t n)
{
    /* Find the length L of the base‑5 representation of the nth term. */
    idx_t cum = 0;
    idx_t L;
    for (L = 2; ; ++L) {
        idx_t cnt = (idx_t)(L - 1) * (idx_t)(L - 1) * power3(L - 2);
        if (cum + cnt >= n) {
            break;
        }
        cum += cnt;
    }

    /* Index within this length, zero‑based. */
    idx_t k = n - cum - 1;

    /* Construct the number digit by digit. */
    seq_t result = 0;
    bool zero_used = false;
    bool one_used = false;

    for (idx_t pos = 0; pos < L; ++pos) {
        /* Iterate over allowed digits in ascending order. */
        for (int d = 0; d <= 4; ++d) {
            if (pos == 0 && d == 0)          /* no leading zero */
                continue;
            if (d == 0 && zero_used)        /* only one zero allowed */
                continue;
            if (d == 1 && one_used)         /* only one one allowed */
                continue;

            /* Compute how many completions are possible after choosing d. */
            bool nz = zero_used || (d == 0);
            bool no = one_used || (d == 1);
            idx_t rem = L - pos - 1;
            int zeros_needed = nz ? 0 : 1;
            int ones_needed = no ? 0 : 1;

            idx_t cnt = 0;
            if (zeros_needed + ones_needed > (int)rem) {
                cnt = 0;
            } else if (zeros_needed == 1 && ones_needed == 1) {
                cnt = rem * (rem - 1) * power3(rem - 2);
            } else if (zeros_needed == 1 || ones_needed == 1) {
                cnt = rem * power3(rem - 1);
            } else { /* both already used */
                cnt = power3(rem);
            }

            if (k < cnt) {
                /* Choose this digit and proceed. */
                result = result * (seq_t)5 + (seq_t)d;
                zero_used = nz;
                one_used = no;
                break;
            } else {
                k -= cnt;
            }
        }
    }

    return result;
}
