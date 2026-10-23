/*
 * A045266  Numbers whose base-5 representation contains exactly one 2 and no
 *          3's.
 * https://oeis.org/A045266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose base‑5 representation * contains
 *            exactly one digit '2' and no digit '3'
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Precompute powers of 3 up to a generous limit (enough for n <= 10000). */
static seq_t pow3[64];
static bool init_pow3 = false;
static void init_powers(void)
{
    if (init_pow3) return;
    pow3[0] = (seq_t)1;
    for (int i = 1; i < 64; ++i)
        pow3[i] = pow3[i-1] * (seq_t)3;
    init_pow3 = true;
}

/* Count of k‑digit numbers (first digit non‑zero) that satisfy the
 * “exactly one 2, no 3” rule.  For k = 1 the count is 1 (just the digit 2). */
static idx_t count_k_digits(idx_t k)
{
    if (k == 1) return 1;
    /* For k >= 2: 3^(k-2) * (2*k + 1) */
    init_powers();
    seq_t cnt = pow3[k-2] * (seq_t)(2*k + 1);
    return (idx_t)cnt;   /* fits in idx_t for the used range */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* Undefined for n = 0, return 0 safely */

    init_powers();

    /* 1. Find the digit‑length k such that cumulative count >= n. */
    idx_t k = 1;
    idx_t cum = 0;                /* numbers counted so far */
    idx_t cnt = count_k_digits(k);
    while (cum + cnt < n) {
        cum += cnt;
        ++k;
        cnt = count_k_digits(k);
    }

    /* 2. Rank of the desired number within the k‑digit bucket (0‑based). */
    idx_t rank = n - cum - 1;

    /* 3. Unrank the number digit by digit. */
    bool used2 = false;           /* whether the digit '2' has been placed */
    seq_t value = 0;

    for (idx_t pos = 0; pos < k; ++pos) {
        idx_t rem = k - pos - 1;  /* remaining positions after current */
        /* Determine the set of allowed digits for this position. */
        /* All positions allow digits {0,1,2,4} except the first cannot be 0. */
        const int first_options[]   = {1, 2, 4};
        const int other_options[]   = {0, 1, 2, 4};
        const int *opts = (pos == 0) ? first_options : other_options;
        const size_t opt_cnt = (pos == 0) ? 3 : 4;

        for (size_t oi = 0; oi < opt_cnt; ++oi) {
            int d = opts[oi];
            if (used2 && d == 2) continue;   /* second '2' not allowed */

            bool new_used2 = used2 || (d == 2);

            /* Count completions for the remaining positions. */
            seq_t comps;
            if (new_used2) {
                /* All remaining digits cannot be 2.  Options per digit = 3. */
                comps = pow3[rem];
            } else {
                /* Still need one 2 somewhere later. */
                if (rem == 0) {
                    comps = 0;
                } else {
                    /* Choose one of the remaining positions for the '2'
                       and set the others to 3 choices. */
                    comps = (seq_t)rem * pow3[rem - 1];
                }
            }
            idx_t comps_idx = (idx_t)comps;

            if (rank >= comps_idx) {
                rank -= comps_idx;
                continue;
            } else {
                /* Take this digit. */
                value = value * (seq_t)5 + (seq_t)d;
                used2 = new_used2;
                break;
            }
        }
    }

    return value;
}
