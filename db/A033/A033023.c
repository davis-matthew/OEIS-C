/*
 * A033023  Numbers whose base-10 expansion has no run of digits with length <
 *          2.
 * https://oeis.org/A033023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 203, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   nCr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th number whose decimal expansion has no run of
 * a single digit (every run of equal digits has length ≥ 2).
 * The algorithm enumerates numbers by increasing length, counts how
 * many such numbers of a given length exist, finds the length that
 * contains the n-th term, then depth‑first searches in lexicographic
 * order to extract the exact term.
 * Complexity:  O(a(n)) for enumeration, about O(n * log n) for the
 *      counts of each length.
 */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/nCr.h"

/* Count how many valid numbers of length L exist.  The result fits
 * into idx_t because it grows at most polynomially in L. */
static idx_t count_length(idx_t L)
{
    idx_t total = 0;
    int Lint = (int)L;                 /* L is small enough for int */
    int max_k = Lint / 2;              /* maximum number of runs */
    for (int k = 1; k <= max_k; ++k) {
        idx_t comp = (idx_t)nCr((seq_t)(Lint - k - 1), k - 1); /* compositions */
        idx_t pow9 = 1;
        for (int t = 0; t < k; ++t) pow9 *= 9;                /* 9 choices per run */
        total += comp * pow9;
    }
    return total;
}

/* Depth‑first search to generate numbers of fixed length L, in
 * ascending lexicographic order, and find the target-th number.
 * Parameters:
 *   L        – length of the numbers
 *   target   – zero‑based index within this length
 *   found    – counter of how many valid numbers have been generated so far
 *   res      – the result value when found
 *   pos      – current position (0‑based)
 *   last_d   – digit of current run (-1 only for pos==0)
 *   run_len  – length of the current run
 *   val      – numeric value built so far
 * Returns true when the target has been found. */
static bool dfs_generate(idx_t L, seq_t target,
                         idx_t *found, seq_t *res,
                         idx_t pos, int last_d, int run_len,
                         seq_t val)
{
    if (pos == L) {
        if (run_len >= 2) {
            ++(*found);
            if (*found == (idx_t)(target + 1)) {
                *res = val;
                return true;
            }
        }
        return false;
    }

    if (pos == 0) {                /* first digit cannot be zero */
        for (int d = 1; d <= 9; ++d) {
            if (dfs_generate(L, target, found, res,
                             pos + 1, d, 1,
                             val * 10 + d))
                return true;
        }
    } else {
        idx_t rem = L - pos;
        for (int d = 0; d <= 9; ++d) {
            if (d == last_d) {          /* continue current run */
                if (dfs_generate(L, target, found, res,
                                 pos + 1, last_d,
                                 run_len + 1,
                                 val * 10 + d))
                    return true;
            } else {                    /* start new run */
                if (run_len >= 2 && rem >= 2) {
                    if (dfs_generate(L, target, found, res,
                                     pos + 1, d, 1,
                                     val * 10 + d))
                        return true;
                }
            }
        }
    }
    return false;
}

/* Main entry function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n==0 */

    /* Find the length L of the desired number. */
    idx_t L = 2;
    idx_t cumulative = 0;
    idx_t nseq = (idx_t)n;           /* n fits in idx_t for the tested range */

    for (;;) {
        idx_t cnt = count_length(L);
        if (cumulative + cnt >= nseq) break;
        cumulative += cnt;
        ++L;
    }

    /* Zero‑based offset within numbers of length L. */
    idx_t target_offset = nseq - cumulative - 1;

    /* Generate the target-th number of this length. */
    idx_t found = 0;
    seq_t result = 0;
    dfs_generate(L, (seq_t)target_offset, &found, &result,
                 0, -1, 0, 0);

    return result;
}
