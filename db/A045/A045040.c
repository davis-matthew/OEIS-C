/*
 * A045040  Numbers whose base-4 representation contains no 0's and exactly
 *          three 2's.
 * https://oeis.org/A045040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 2561, u32 n <= 6000, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑4 representation contains no 0’s and exactly three 2’s. */
/* The algorithm: 1. Find the base‑4 length `l` of the n‑th number by
   accumulating the counts of admissible strings.
   2. Unrank the lexicographically ordered strings of that length
      that contain exactly three twos.
   3. Convert the resulting base‑4 digit string to a decimal value.   */
#include <stddef.h>

/* combinatorial helper: C(n,k) with k ≤ n; result fits in seq_t.  */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry to minimise work */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)(n - k + i));
        res = res / ((seq_t)i);        /* exact division – binomial is integer */
    }
    return res;
}

/* power of two: 2^exp, computed multiplicatively.  */
static seq_t pow2_exp(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i) r = r * 2;
    return r;
}

/* number of admissible strings of length `rem_pos` that contain
   exactly `rem_twos` twos (rem_twos ≤ rem_pos).  */
static seq_t count_strings(idx_t rem_pos, idx_t rem_twos)
{
    if (rem_twos > rem_pos) return 0;
    return binom_small(rem_pos, rem_twos) * pow2_exp(rem_pos - rem_twos);
}

seq_t a(idx_t n)
{
    /* step 1: find the length `l` of the base‑4 representation */
    seq_t cum = 0;        /* cumulative count of all shorter lengths   */
    idx_t l   = 3;        /* minimal length (must have 3 twos)           */
    for (;; ++l) {
        seq_t cnt_len = count_strings(l, 3);   /* C(l,3)*2^(l-3) */
        if (cum + cnt_len >= (seq_t)n) break;
        cum += cnt_len;
    }

    /* step 2: unrank among strings of length l   */
    seq_t idx  = (seq_t)n - cum - 1;          /* 0‑based index within this length */
    seq_t val  = 0;
    idx_t rem_twos = 3;                       /* twos still to place            */

    for (idx_t pos = 0; pos < l; ++pos) {
        for (int d = 1; d <= 3; ++d) {
            if (d == 2 && rem_twos == 0) continue;   /* cannot place more 2's */
            idx_t rem_twos_next = rem_twos - (d == 2 ? 1 : 0);
            idx_t rem_pos       = l - pos - 1;
            seq_t cnt = count_strings(rem_pos, rem_twos_next);

            if (idx >= cnt) {
                idx -= cnt;   /* skip entire block */
            } else {
                val = val * ((seq_t)4) + (seq_t)d;
                rem_twos = rem_twos_next;
                break;       /* go to next position */
            }
        }
    }
    return val;
}
