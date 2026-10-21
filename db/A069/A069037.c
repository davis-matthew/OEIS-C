/*
 * A069037  Smallest multiple of 4 with digit sum 4^n.
 * https://oeis.org/A069037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest multiple of 4 with digit sum 4^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"          /* pow4(idx_t) computes 4^n as seq_t */
#include <stdbool.h>                /* for bool type */

/* compute the smallest multiple of 4 whose digit sum equals 4^n */
seq_t a(idx_t n)
{
    /* digit sum target */
    seq_t S = pow4(n);

    seq_t best = 0;
    bool has_best = false;

    /* 25 candidates for the last two digits : 0,4,8,...,96 (multiple of 4) */
    for (int t = 0; t <= 96; t += 4) {
        /* digit sum of the two‑digit suffix */
        int sum_t = (t < 10) ? t : (t / 10 + t % 10);
        if (S < (seq_t)sum_t)
            continue;                  /* suffix sum exceeds target */

        seq_t R = S - (seq_t)sum_t;    /* remaining sum to distribute in the higher digits */

        seq_t val;

        if (R == 0) {                  /* number consists solely of the suffix */
            val = (seq_t)t;
        } else {
            /* minimal number with digit sum R (highest possible length) */
            seq_t quotient = R / 9;
            int rem = (int)(R % 9);    /* 0 <= rem <= 8 */
            seq_t len_q = (rem == 0) ? quotient : quotient + 1;

            /* leading digit of the high part */
            int first_digit = (rem == 0) ? 9 : rem;

            /* len_q must fit into an unsigned long long for the loop */
            unsigned long long len = (unsigned long long)len_q;

            seq_t high = first_digit;
            for (unsigned long long i = 1; i < len; ++i)
                high = high * 10 + 9;

            /* append the two‑digit suffix */
            val = high * 100 + (seq_t)t;
        }

        if (!has_best || val < best) {
            best = val;
            has_best = true;
        }
    }

    return best;
}
