/*
 * A161821  a(n) = the smallest integer > n such that both n and a(n), when
 *          represented in binary, contain the same types of runs of 0's, the
 *          runs being in any order.
 * https://oeis.org/A161821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9224 (2026-11-01)
 * Helpers:   bit_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: a(n) -- the smallest integer > n whose binary representation
   has exactly the same multiset of zero‑run lengths as that of n, in any
   order (runs may appear in a different sequence). */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/bit_length.h"

/* Simple insertion sort for a small array of seq_t values. */
static void sort_runs(seq_t runs[], size_t n)
{
    for (size_t i = 1; i < n; ++i) {
        seq_t key = runs[i];
        size_t j = i;
        while (j > 0 && runs[j - 1] > key) {
            runs[j] = runs[j - 1];
            --j;
        }
        runs[j] = key;
    }
}

/* Compute the lengths of runs of consecutive zeros in the binary
   representation of x.  The lengths are stored in runs[0..*cnt-1].
   The array must be large enough to hold all runs (bit length of x). */
static void get_zero_runs(seq_t x, seq_t runs[], size_t *cnt)
{
    unsigned bl = bit_length(x);
    if (bl == 0) {
        *cnt = 0;
        return;
    }

    size_t zeros = 0;
    size_t index = 0;
    for (int i = (int)bl - 1; i >= 0; --i) {
        bool bit = ((x >> i) & 1u) != 0;
        if (bit) {
            if (zeros > 0) {
                runs[index++] = zeros;
                zeros = 0;
            }
        } else {
            ++zeros;
        }
    }
    if (zeros > 0) {
        runs[index++] = zeros;
    }
    *cnt = index;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* Precompute zero‑run multiset for N. */
    seq_t nRuns[128];
    size_t nCnt;
    get_zero_runs(N, nRuns, &nCnt);
    sort_runs(nRuns, nCnt);

    seq_t m = N + 1;
    while (1) {
        seq_t mRuns[128];
        size_t mCnt;
        get_zero_runs(m, mRuns, &mCnt);
        if (mCnt != nCnt) {
            ++m;
            continue;
        }
        sort_runs(mRuns, mCnt);
        bool equal = true;
        for (size_t i = 0; i < nCnt; ++i) {
            if (mRuns[i] != nRuns[i]) {
                equal = false;
                break;
            }
        }
        if (equal) {
            return m;
        }
        ++m;
    }
}
