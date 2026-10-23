/*
 * A026419  a(n) = least positive integer > a(n-1) and not a(i)*a(j) for
 *          1<=i<j<=n, with initial terms 1,4.
 * https://oeis.org/A026419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer > a(n-1) not equal to a(i)*a(j)
 *            for 1 ≤ i < j ≤ n‑1, with a(1)=1, a(2)=4. The sequence is
 *            strictly increasing
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

#ifdef __cplusplus
#include <vector>
#include <algorithm>

/* Storage for the sequence (C++).  We use std::vector to avoid manual
   allocation of non‑trivial types. */
static std::vector<seq_t> seq;

/* Binary search for a value in the sequence. */
static bool contains(const seq_t &x)
{
    auto it = std::lower_bound(seq.begin(), seq.end(), x);
    return it != seq.end() && *it == x;
}

#else
/* Storage for the sequence (C). */
static seq_t *seq = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

/* Ensure capacity at least 'need'.  Uses malloc/realloc. */
static void seq_ensure(size_t need)
{
    if (seq_cap >= need)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    if (new_cap < need)
        new_cap = need;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Binary search for a value in the sequence. */
static bool contains(seq_t val)
{
    size_t low = 0, high = seq_len;
    while (low < high) {
        size_t mid = (low + high) >> 1;
        if (seq[mid] < val)
            low = mid + 1;
        else
            high = mid;
    }
    return (low < seq_len && seq[low] == val);
}
#endif  /* __cplusplus */

seq_t a(idx_t n)
{
    if (n == 0)               /* not used, but defined for safety */
        return (seq_t)0;

#ifdef __cplusplus
    /* Initialise the first two terms. */
    if (seq.empty()) {
        seq.reserve(16);
        seq.push_back((seq_t)1);
        seq.push_back((seq_t)4);
    }
#else
    if (seq_len == 0) {
        seq_ensure(2);
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)4;
        seq_len = 2;
    }
#endif

    size_t target = (size_t)n;

#ifdef __cplusplus
    while (seq.size() < target) {
        seq_t cand = seq.back() + 1;

        /* Find the next admissible candidate. */
        while (true) {
            seq_t limit = floor_sqrt(cand);        /* sqrt(cand) */
            bool forbidden = false;

            /* Check all i such that a[i] ≤ sqrt(cand). */
            for (size_t i = 0; i < seq.size(); ++i) {
                if (seq[i] > limit)
                    break;
                if (cand % seq[i] == 0) {
                    seq_t y = cand / seq[i];
                    if (y > seq[i] && contains(y)) {
                        forbidden = true;
                        break;
                    }
                }
            }
            if (!forbidden)
                break;                     /* admissible */
            ++cand;                          /* try next integer */
        }

        seq.push_back(cand);
    }

    return seq[n - 1];
#else /* C version */
    while (seq_len < target) {
        seq_t cand = seq[seq_len - 1] + 1;

        while (true) {
            seq_t limit = floor_sqrt(cand);
            bool forbidden = false;

            for (size_t i = 0; i < seq_len; ++i) {
                if (seq[i] > limit)
                    break;
                if (cand % seq[i] == 0) {
                    seq_t y = cand / seq[i];
                    if (y > seq[i] && contains(y)) {
                        forbidden = true;
                        break;
                    }
                }
            }
            if (!forbidden)
                break;
            ++cand;
        }

        seq_ensure(seq_len + 1);
        seq[seq_len++] = cand;
    }

    return seq[n - 1];
#endif
}
