/*
 * A070244  Largest palindrome using minimum number of digits with a digit sum
 *          = n.
 * https://oeis.org/A070244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = largest palindrome with the smallest possible number of
 *            digits that has digit sum equal to n
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm:
   * Find the minimal length L that permits a palindrome of sum n.
     For even L, n must be even and n/2 must be between 1 and 9*(L/2).
     For odd L, there must exist a middle digit 0..9 such that the
     remaining half sum is between 1 and 9*((L-1)/2) (or 0 when L=1).
   * Construct the palindrome of that length with the largest
     possible value.  The left half is built greedily: at each
     position choose the largest digit that still allows the
     remaining sum to be distributed over the remaining positions,
     respecting the leading‑digit constraint.
*/
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle single‑digit cases directly. */
    if (n <= 9) return (seq_t)n;

    /* ----- Find minimal admissible length L ----- */
    idx_t L = 2;          /* start with the smallest length >1 */
    idx_t k = 0;          /* number of mirrored positions (half‑width) */
    bool is_odd = false;  /* will be set once a feasible length is found */
    idx_t chosen_mid = 0; /* middle digit for odd length */

    for (;; ++L) {
        if ((L & 1) == 0) {                 /* even length */
            if (n & 1) continue;            /* sum must be even */
            k = L >> 1;
            seq_t half = (seq_t)n >> 1;      /* n / 2 */
            if (half >= 1 && half <= (seq_t)9 * k) {
                is_odd = false;
                break;
            }
        } else {                             /* odd length */
            k = (L - 1) >> 1;
            bool ok = false;
            for (int mid = 0; mid <= 9 && !ok; ++mid) {
                if (((n - (idx_t)mid) & 1)) continue;   /* (n-mid) must be even */
                seq_t half = ((seq_t)n - mid) >> 1;     /* (n-mid)/2 */
                if (k == 0) {
                    if (half == 0) { ok = true; chosen_mid = mid; break; }
                } else if (half >= 1 && half <= (seq_t)9 * k) {
                    ok = true;
                    chosen_mid = mid;
                    break;
                }
            }
            if (ok) {
                is_odd = true;
                break;
            }
        }
    }

    /* ----- Construct the palindrome ----- */
    seq_t result = 0;
    static uint8_t left[2000];  /* buffer large enough for all n <= 10000 */

    if (is_odd) {
        seq_t sum_needed = (((seq_t)n - chosen_mid) >> 1);   /* (n-mid)/2 */

        for (idx_t i = 0; i < k; ++i) {
            idx_t rem = k - i - 1;
            seq_t lower = sum_needed - (seq_t)9 * rem;
            if (i == 0 && lower < 1) lower = 1;
            if (lower < 0) lower = 0;
            if (lower > 9) lower = 9;

            seq_t ai = sum_needed < 9 ? sum_needed : 9;
            if (ai < lower) ai = lower;

            left[i] = (uint8_t)ai;
            sum_needed -= ai;
        }

        /* left half */
        for (idx_t i = 0; i < k; ++i)
            result = result * 10 + (seq_t)left[i];
        /* middle digit */
        result = result * 10 + (seq_t)chosen_mid;
        /* mirrored right half */
        for (idx_t i = k; i > 0; --i)
            result = result * 10 + (seq_t)left[i - 1];
    } else {
        seq_t sum_needed = (seq_t)n >> 1;  /* n / 2 */

        for (idx_t i = 0; i < k; ++i) {
            idx_t rem = k - i - 1;
            seq_t lower = sum_needed - (seq_t)9 * rem;
            if (i == 0 && lower < 1) lower = 1;
            if (lower < 0) lower = 0;
            if (lower > 9) lower = 9;

            seq_t ai = sum_needed < 9 ? sum_needed : 9;
            if (ai < lower) ai = lower;

            left[i] = (uint8_t)ai;
            sum_needed -= ai;
        }

        /* left half */
        for (idx_t i = 0; i < k; ++i)
            result = result * 10 + (seq_t)left[i];
        /* mirrored right half */
        for (idx_t i = k; i > 0; --i)
            result = result * 10 + (seq_t)left[i - 1];
    }

    return result;
}
