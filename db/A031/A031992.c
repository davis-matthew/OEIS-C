/*
 * A031992  Duplicate of A023749.
 * https://oeis.org/A031992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 58, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers with non‑decreasing digits in base 7
   Equivalent to: a(n) is the n-th positive integer whose base‑7
   representation satisfies d_1 ≤ d_2 ≤ … ≤ d_k   (d_i in {0,…,6}),
   with a(1)=0.  */
/*   References: A031992 – OEIS */

#include <stdint.h>

static idx_t binom_u64(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* the division is exact */
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* a(1) = 0 */
    if (n == 1) return (seq_t)0;

    /* work with the (n‑1)-th positive term */
    idx_t pos = n - 1;

    idx_t cumulative = 0;
    idx_t k = 0;                 /* number of base‑7 digits */

    while (1) {
        ++k;
        idx_t count_k = binom_u64(k + 5, k);  /* C(k+5, k) */
        if (pos <= cumulative + count_k) {
            /* position (0‑based) within the k‑digit block */
            pos -= cumulative + 1;

            uint8_t digits[20];   /* k ≤ 10 for all n < 10 000 */
            uint8_t prev = 1;     /* first digit must be ≥ 1 for n>0 */

            for (idx_t i = 0; i < k; ++i) {
                idx_t rest = k - i - 1;
                for (uint8_t v = prev; v <= 6; ++v) {
                    idx_t ways = binom_u64(rest + (6 - v), rest);
                    if (pos < ways) {
                        digits[i] = v;
                        prev = v;
                        break;
                    }
                    pos -= ways;
                }
            }

            seq_t val = 0;
            for (idx_t i = 0; i < k; ++i) {
                val = val * (seq_t)7 + (seq_t)digits[i];
            }
            return val;
        } else {
            cumulative += count_k;
        }
    }
}
