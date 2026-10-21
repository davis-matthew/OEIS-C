/*
 * A095115  a(1)=1. Given a(1),...,a(n-1), to find a(n), let S = {a(1), ...,
 *          a(n-1), |a(2)-a(1)|, ..., |a(n-1)-a(n-2)|}. Let d be the smallest
 *          positive integer not in S. Then a(n) is the smallest one of
 *          a(n-1)-d and a(n-1)+d which is a positive integer not in S union
 *          {d}.
 * https://oeis.org/A095115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = a(n‑1) ± d where * a(1) = 1 * S = {a(i)} ∪
 *            {|a(i)‑a(i‑1)|} * d = mex(S) * a(n) is the smallest positive
 *            integer (≠ d) in {a(n‑1)‑d, a(n‑1)+d} * that does not belong to
 *            S ∪ {d}; if neither is valid the sequence * terminates. The
 *            sequence is defined for all n ≥ 1 until it * terminates
 * Range:     u8 n <= 27, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use static arrays of fixed maximal capacity (20000).  This
 * avoids any dynamic allocation and works both in C (where seq_t is
 * a built‑in unsigned integer type) and in C++ (where seq_t may be an
 * arbitrary‑precision class).  The algorithm is O(n²) time but runs
 * comfortably for n up to 10 000. */

#include <stdbool.h>
#include <stddef.h>   /* size_t */

#ifndef MAX_CAP
#define MAX_CAP 20000   /* must be ≥ requested maximum n + small margin */
#endif

static seq_t seq[MAX_CAP];
static seq_t used[MAX_CAP];
static seq_t diffs[MAX_CAP];
static size_t seq_len   = 0;
static size_t used_len  = 0;
static size_t diffs_len = 0;
static bool   terminated = false;

static inline bool contains_used(seq_t v)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used[i] == v) return true;
    return false;
}

static inline bool contains_diffs(seq_t v)
{
    for (size_t i = 0; i < diffs_len; ++i)
        if (diffs[i] == v) return true;
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n==0 */

    /* initialise first term if needed */
    if (!terminated && seq_len == 0) {
        seq[0]   = (seq_t)1;
        seq_len  = 1;
        used[0]  = (seq_t)1;
        used_len = 1;
    }

    /* extend sequence until we have at least n terms or it terminates */
    while (!terminated && seq_len < n) {
        if (seq_len >= MAX_CAP) {    /* safety guard */
            terminated = true;
            break;
        }

        seq_t prev = seq[seq_len - 1];
        seq_t d    = (seq_t)1;

        /* find mex of S */
        for (;;) {
            bool ok = true;
            if (contains_used(d) || contains_diffs(d)) {
                ok = false;
            }
            if (ok) break;
            ++d;
        }

        seq_t cand1 = (prev > d) ? (prev - d) : (seq_t)0; /* prev > d guarantees no underflow */
        seq_t cand2 = prev + d;                           /* addition may overflow; such value is not valid when checked */

        seq_t chosen = (seq_t)0;

        /* test cand1 first, then cand2 if needed */
        if (cand1 > (seq_t)0 && cand1 != d &&
            !contains_used(cand1) && !contains_diffs(cand1)) {
            chosen = cand1;
        } else if (cand2 > (seq_t)0 && cand2 != d &&
                   !contains_used(cand2) && !contains_diffs(cand2)) {
            chosen = cand2;
        }

        if (chosen == (seq_t)0) {  /* no valid candidate → terminate */
            terminated = true;
            break;
        }

        /* append chosen term */
        seq[seq_len] = chosen;
        seq_len++;

        used[used_len] = chosen;
        used_len++;

        seq_t diff = (prev >= chosen) ? (prev - chosen) : (chosen - prev);
        diffs[diffs_len] = diff;
        diffs_len++;
    }

    if (seq_len >= n) return seq[n - 1];
    return (seq_t)0;   /* requested term does not exist before sequence termination */
}
