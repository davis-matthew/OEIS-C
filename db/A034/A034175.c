/*
 * A034175  a(n) is minimal such that a(n)+a(n-1) is a square and a(n) is not
 *          in {a(0), ..., a(n-1)}.
 * https://oeis.org/A034175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9847 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0.
   For n > 0, a(n) is the smallest non‑negative integer not yet used
   such that a(n‑1) + a(n) is a perfect square.  The sequence is
   generated lazily and memoized, so calls can arrive in any order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t *seq = NULL;          /* memoized sequence values */
static size_t seq_len = 0;          /* number of values stored */
static size_t seq_cap = 0;          /* capacity of seq array */

static bool *used = NULL;           /* bitmap of used numbers */
static size_t used_cap = 0;         /* capacity of used bitmap */

/* ensure the used bitmap can address index 'x' */
#define ENSURE_USED(x)                                     \
    do {                                                   \
        idx_t _req = (x);                                  \
        if ((size_t)_req >= used_cap) {                    \
            size_t _new = used_cap > 0 ? used_cap * 2 : 16;\
            while ((size_t)_req >= _new) _new <<= 1;       \
            bool *tmp = (bool *)realloc(used, _new * sizeof(bool)); \
            if (!tmp) exit(1);                            \
            memset(tmp + used_cap, 0, (_new - used_cap) * sizeof(bool)); \
            used = tmp;                                    \
            used_cap = _new;                               \
        }                                                  \
    } while (0)

seq_t a(idx_t n)
{
    /* grow seq array if necessary */
    if ((size_t)n >= seq_cap) {
        size_t _new = seq_cap > 0 ? seq_cap * 2 : 16;
        while ((size_t)n >= _new) _new <<= 1;
        seq_t *tmp = mem_resize(seq_t, seq, _new);
        if (!tmp) exit(1);
        seq   = tmp;
        seq_cap = _new;
    }

    /* compute terms up to index n */
    while (seq_len <= n) {
        if (seq_len == 0) { /* a(0) = 0 */
            seq[0] = 0;
            ENSURE_USED(0);
            used[0] = true;
            seq_len = 1;
            continue;
        }

        seq_t prev = seq[seq_len - 1];

        /* ceil(sqrt(prev)) */
        seq_t m = floor_sqrt(prev);
        if (m * m < prev) ++m;

        seq_t cand;
        /* search for the smallest unused candidate with prev + cand = square */
        while (1) {
            cand = m * m - prev;          /* candidate value */
            idx_t idx = (idx_t)cand;
            ENSURE_USED(idx);
            if (!used[idx]) break;
            ++m;
        }

        /* record candidate */
        idx_t idx = (idx_t)cand;
        ENSURE_USED(idx);
        used[idx] = true;
        seq[seq_len++] = cand;
    }

    return seq[n];
}
