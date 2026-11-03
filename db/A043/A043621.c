/*
 * A043621  Numbers whose base-7 representation has exactly 6 runs.
 * https://oeis.org/A043621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer whose base‑7 representation has exactly six runs */
#include <stdbool.h>

/* Recursive enumeration of numbers of fixed length L with 5 digit changes
   (i.e. exactly 6 runs).  Digits are generated in lexicographic order
   and the offset‑th valid number is stored in *res. */
static void gen_seq(int pos, int prev, int changes_left,
                    seq_t val, idx_t L,
                    idx_t *offset, idx_t *cnt,
                    seq_t *res, bool *done) {
    if (*done) return;
    if (pos == (int)L) {
        if (changes_left == 0) {
            (*cnt)++;
            if (*cnt == *offset) {
                *res = val;
                *done = true;
            }
        }
        return;
    }
    int rem = (int)L - pos;          /* positions still to fill (incl. this one) */
    int d_start = (pos == 0) ? 1 : 0;
    for (int d = d_start; d <= 6; ++d) {
        int n_changes = changes_left;
        if (pos > 0 && d != prev) n_changes--;
        if (n_changes < 0) continue;
        int rem_after = (int)L - pos - 1;
        if (n_changes > rem_after) continue;   /* cannot complete the required changes */
        seq_t new_val = val * (seq_t)7 + (seq_t)d;
        gen_seq(pos + 1, d, n_changes, new_val, L,
                offset, cnt, res, done);
        if (*done) return;
    }
}

/* Binomial coefficient C(m,5) using 128‑bit arithmetic */
static unsigned __int128 binom5(idx_t m) {
    if (m < 5) return 0;
    unsigned __int128 r = 1;
    r *= m;          /* m */
    r *= (m - 1);    /* m*(m-1) */
    r *= (m - 2);    /* m*(m-1)*(m-2) */
    r *= (m - 3);    /* m*(m-1)*(m-2)*(m-3) */
    r *= (m - 4);    /* m*(m-1)*(m-2)*(m-3)*(m-4) */
    r /= 120;        /* divide by 5! = 120 */
    return r;
}

seq_t a(idx_t n) {
    /* Find the length L of the n‑th number */
    idx_t cum = 0;
    idx_t L = 6;
    idx_t offset = 0;
    while (1) {
        unsigned __int128 comb = binom5(L - 1);                 /* C(L-1,5) */
        unsigned __int128 count128 = comb * (unsigned __int128)46656; /* 6*6^5 = 46656 */
        if (count128 >= (unsigned __int128)(n - cum)) {
            offset = n - cum;
            break;
        }
        cum += (idx_t)count128;
        ++L;
    }

    /* Generate the offset‑th number of length L */
    seq_t result = 0;
    idx_t counter = 0;
    bool done = false;
    gen_seq(0, 0, 5, 0, L, &offset, &counter, &result, &done);
    return result;
}
