/*
 * A043396  Numbers whose base-7 representation contains exactly four 0's.
 * https://oeis.org/A043396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose base‑7 representation * contains
 *            exactly four 0's
 * Range:     u8 none, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow6_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow6_exp.h"

/* ---------- helpers ---------- */

/* C(n,4) for n >= 4, 0 otherwise */
static inline idx_t binom4(idx_t n)
{
    if (n < 4)
        return 0;
    return n * (n - 1) * (n - 2) * (n - 3) / 24;
}

/* C(n,k) for k <= 4 (used for small k only) */
static inline idx_t binom_small(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    switch (k)
    {
        case 0: return 1;
        case 1: return n;
        case 2: return n * (n - 1) / 2;
        case 3: return n * (n - 1) * (n - 2) / 6;
        case 4: return n * (n - 1) * (n - 2) * (n - 3) / 24;
        default: return 0;
    }
}

/* ---------- main function ---------- */

seq_t a(idx_t n)
{
    /* n >= 1; behaviour for n < 1 is undefined */
    idx_t remaining = n;          /* numbers left to skip            */
    idx_t len = 5;                /* minimal length (4 zeros + 1 nonzero)    */

    /* Find length L that contains the n-th number */
    for (;; ++len)
    {
        idx_t count_len = binom4(len - 1) * pow6_exp(len - 4);
        if (remaining <= count_len)
            break;
        remaining -= count_len;
    }

    /* Now we are looking at numbers of length 'len' */
    idx_t rank = remaining - 1;          /* 0‑based index within this length   */
    int zeros_left = 4;                  /* zeros still to place              */
    seq_t result = 0;                    /* building the number in base 10     */

    for (idx_t pos = 0; pos < len; ++pos)
    {
        int start_digit = (pos == 0) ? 1 : 0;   /* first digit cannot be 0       */

        for (int d = start_digit; d <= 6; ++d)
        {
            if (d == 0 && zeros_left == 0)
                continue;                     /* cannot place more zeros       */

            int zeros_used = (d == 0) ? 1 : 0;
            int zeros_needed = zeros_left - zeros_used;
            idx_t rem_pos = len - pos - 1;

            /* Count of completions with this prefix */
            idx_t count = 0;
            if (zeros_needed >= 0 && zeros_needed <= rem_pos)
                count = binom_small(rem_pos, zeros_needed) *
                        pow6_exp(rem_pos - zeros_needed);

            if (rank >= count)
            {
                rank -= count;               /* skip these numbers           */
                continue;
            }

            /* Choose this digit and proceed */
            result = result * 7 + (seq_t)d;
            zeros_left -= zeros_used;
            break;
        }
    }

    return result;
}
