/*
 * A107781  a(1)=10, a(n) = smallest integer not previously used which
 *          contains a digit from a(n-1).
 * https://oeis.org/A107781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 10 a(n) = smallest integer not previously used that
 *            contains at least one digit from a(n‑1) (n ≥ 2)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   digit_present, fill_allowed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_present.h"
#include "../utils/fill_allowed.h"

/* Maximum number of terms that may be stored.
   200 000 is comfortably larger than the 10 000 used in tests. */
#define MAX_TERMS 200000

/* Storage for the sequence (1‑based in comments, 0‑based in code).   */
static seq_t seq_values[MAX_TERMS];
static size_t seq_len = 0;

/* Storage for values already used.  Linear search will be used
   to test whether a value has appeared before. */
static seq_t used_vals[MAX_TERMS];
static size_t used_len = 0;

/* For each decimal digit d, min_unused[d] is the current
   smallest integer that has not yet been used and contains digit d. */
static seq_t min_unused[10];

/* ----------------------------------------------------------------
   Linear test: does val occur among the used values?                 */
static bool used_contains(seq_t val)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used_vals[i] == val)
            return true;
    return false;
}

/* ----------------------------------------------------------------
   Advance min_unused[d] to the next unused integer that contains d.  */
static void advance_min_unused_for_digit(int d)
{
    seq_t cur = min_unused[d];
    while (used_contains(cur) || !digit_present(cur, d))
        ++cur;
    min_unused[d] = cur;
}

/* ----------------------------------------------------------------
   Record that val has now been used.                                 */
static void record_used(seq_t val)
{
    if (used_len < MAX_TERMS) {
        used_vals[used_len++] = val;
    }
}

/* ----------------------------------------------------------------
   Compute up to the n-th term (n is 1‑based).                         */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined, but harmless          */

    /* Lazy initialization on first call.                     */
    static bool first_call = true;
    if (first_call) {
        first_call = false;
        /* Initialise first term */
        seq_values[0] = (seq_t)10;
        seq_len = 1;
        record_used(seq_values[0]);

        /* initialise minimum unused digits to 0 */
        for (int d = 0; d < 10; ++d)
            min_unused[d] = 0;
    }

    /* Grow sequence until we have term n.                       */
    while (seq_len < (size_t)n) {
        /* Determine allowed digits from previous term.           */
        bool allowed[10];
        fill_allowed(seq_values[seq_len - 1], allowed);

        /* Advance candidates for each allowed digit.             */
        for (int d = 0; d < 10; ++d) {
            if (allowed[d])
                advance_min_unused_for_digit(d);
        }

        /* Pick the smallest among the candidates.                 */
        seq_t best = 0;
        bool first_candidate = true;
        for (int d = 0; d < 10; ++d) {
            if (allowed[d]) {
                seq_t cand = min_unused[d];
                if (first_candidate || cand < best) {
                    best = cand;
                    first_candidate = false;
                }
            }
        }

        /* Store the new term.                                      */
        seq_values[seq_len] = best;
        record_used(best);
        ++seq_len;
    }

    return seq_values[n - 1];
}
