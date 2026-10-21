/*
 * A077495  a(n) = smallest k such that the digit sum of 8k is n.
 * https://oeis.org/A077495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 46, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   digit_sum, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A077495: smallest k such that digit sum of 8k == n.
   For n<24 it is computed by a brute‑force search (cached).
   For n≥24 use the closed form:
          a(n) = 125 * A051885(n-24) + 111
   where A051885(s) = minimal integer with digit sum s
   (equals 0 for s=0, otherwise r followed by q nines,
   with q = s/9, r = s%9). */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/digit_sum.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* --------------------------------------------
       Handle first 24 terms by brute force, cached
       -------------------------------------------- */
    static seq_t cache[24];
    static bool ready = false;
    if (n < 24) {
        if (!ready) {
            bool seen[24];
            memset(seen, 0, sizeof(seen));
            idx_t k = 0;
            idx_t found = 0;
            while (found < 24) {
                seq_t prod = (seq_t)k * 8;
                seq_t s = digit_sum(prod);
                if (s < 24 && !seen[(size_t)s]) {
                    cache[(size_t)s] = k;
                    seen[(size_t)s] = true;
                    ++found;
                }
                ++k;
            }
            ready = true;
        }
        return cache[(size_t)n];
    }

    /* --------------------------------------------
       n >= 24: use the formula a(n) = 125*A051885(n-24)+111
       -------------------------------------------- */
    idx_t s = n - 24;         /* target digit sum for A051885 */
    seq_t m = 0;
    if (s != 0) {
        idx_t q = s / 9;          /* number of nines */
        idx_t r = s % 9;          /* leading digit if non‑zero */
        if (r != 0) {
            m = r;                /* start with leading digit r */
        }
        /* append q copies of digit 9: m = m*10 + 9  (q times) */
        for (idx_t i = 0; i < q; ++i) {
            /* multiply by 10 and add 9 */
            m = (m * 10) + 9;
        }
    }
    /* a(n) = 125 * m + 111 */
    seq_t result = mul_small(m, 125) + (seq_t)111;
    return result;
}
