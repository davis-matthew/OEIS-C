/*
 * A045081  Numbers whose base-4 representation contains exactly four 0's and
 *          no 3's.
 * https://oeis.org/A045081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate base‑4 strings of length L that contain exactly
   four 0's, no 3's, and no leading 0.  Numbers are produced in ascending
   order; a(n) is the n‑th such number, offset 1. */

#include <stdbool.h>

/* combinatorial helpers that return values that fit in idx_t. */
static idx_t comb(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - i + 1);
        res = res / i;
    }
    return res;
}

static idx_t pow2(idx_t e)
{
    idx_t r = 1;
    for (idx_t i = 0; i < e; ++i) r <<= 1;
    return r;
}

/* ----------  main sequence function  ---------- */
seq_t a(idx_t n)
{
    /* n is 1‑based.  If called with n==0 behaviour is undefined
       but we guard against it by returning 0. */
    if (n == 0) return 0;

    idx_t remaining = n;          /* how many numbers to skip */
    idx_t length   = 5;           /* minimum possible length of the base‑4 representation */
    while (1) {
        /* Count how many eligible numbers have this length. */
        idx_t cnt = comb(length - 1, 4) * pow2(length - 4);

        if (remaining <= cnt) {
            /* the desired number is among the numbers of this length */
            idx_t rank = remaining - 1;   /* zero‑based rank within this block */
            idx_t zeros_to_place = 4;      /* how many zeros we still need to place */
            seq_t value = 0;               /* accumulator for the numeric value */

            for (idx_t pos = 0; pos < length; ++pos) {
                /* try each possible digit in increasing order: 0,1,2,
                   but we skip 0 as first digit and skip 0 if no zeros left. */
                for (int dig = 0; dig <= 2; ++dig) {
                    if (pos == 0 && dig == 0) continue;   /* no leading zero */
                    if (dig == 0 && zeros_to_place == 0) continue;

                    idx_t rem_pos = length - 1 - pos;
                    idx_t zeros_after = zeros_to_place - (dig == 0 ? 1 : 0);
                    if (zeros_after > rem_pos) continue;    /* impossible to finish */

                    idx_t ways =
                        comb(rem_pos, zeros_after) *
                        pow2(rem_pos - zeros_after);

                    if (rank < ways) {
                        /* this digit is the correct one for the current position */
                        value = value * 4 + (seq_t)dig;
                        if (dig == 0) zeros_to_place--;
                        break;
                    } else {
                        rank -= ways;                      /* skip this block of numbers */
                    }
                }
            }
            return value;
        } else {
            remaining -= cnt;   /* skip this entire block of numbers */
            ++length;           /* consider numbers of the next length */
        }
    }
}
