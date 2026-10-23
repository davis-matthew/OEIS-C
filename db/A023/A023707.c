/*
 * A023707  Numbers with exactly 2 0's in their base 4 expansion.
 * https://oeis.org/A023707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   choose2_saturated, count_rem, mul_saturated, pow3_sat
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with exactly two zero digits in their base‑4 expansion
   (OEIS A023707).  The function returns the n‑th smallest positive
   integer whose base‑4 representation contains exactly two zero
   digits, where a(1) = 16, a(2) = 32, … . */

#include <stdbool.h>
#include "../utils/choose2_saturated.h"
#include "../utils/count_rem.h"
#include "../utils/mul_saturated.h"
#include "../utils/pow3_sat.h"

static inline idx_t count_len(idx_t L, idx_t limit)
{
    if (L < 3) return 0;
    idx_t rem   = L - 3;                     /* positions after leading digit and two zeros */
    idx_t comb  = choose2_saturated(L - 1, limit);   /* choose 2 zeros among the remaining L‑1 positions */
    if (comb == 0) return 0;
    idx_t powv  = pow3_sat(rem, limit);      /* 3^(L-3) for the non‑zero, non‑zero, non‑zero slots */
    idx_t tmp   = mul_saturated(comb, powv, limit);
    return mul_saturated(tmp, (idx_t)3, limit);    /* 3 choices for the leading digit (1,2,3) */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour for indices below offset is arbitrary */

    idx_t limit = n;               /* used for saturation in counting */

    /* Find the digit length L of a(n) */
    idx_t cumulative = 0;
    idx_t L;
    for (L = 3; ; ++L) {
        idx_t cnt = count_len(L, limit);
        if (cnt >= limit) cnt = limit;   /* avoid overflow when adding */
        if (cumulative + cnt >= n) break;
        cumulative += cnt;
    }

    idx_t offset = n - cumulative - 1;   /* zero‑based within the L‑digit block */

    seq_t res         = 0;
    idx_t zeros_left  = 2;

    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t start = (pos == 0) ? 1 : 0;
        for (idx_t d = start; d <= 3; ++d) {
            idx_t zeros_after = zeros_left - (d == 0 ? 1 : 0);
            if (zeros_after < 0) continue;
            idx_t rem = L - pos - 1;
            if (zeros_after > rem) continue;

            idx_t cnt = count_rem(rem, zeros_after, limit);
            if (offset < cnt) {
                res = res * (seq_t)4 + (seq_t)d;
                zeros_left = zeros_after;
                break;
            } else {
                offset -= cnt;
            }
        }
    }
    return res;
}
