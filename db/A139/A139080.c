/*
 * A139080  a(n) = the smallest positive integer not occurring earlier in the
 *          sequence such that floor(max(a(n),a(n-1))/min(a(n),a(n-1))) = 2;
 *          a(1) = 1.
 * https://oeis.org/A139080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = the smallest unused positive integer m * such that
 *            floor(max(m,a(n-1))/min(m,a(n-1))) = 2. * a(1) = 1
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- internal state ---------- */
static seq_t *seq_cache = NULL;   /* 1‑based cache of a(n) */
static size_t seq_len   = 0;      /* number of stored terms         */
static size_t seq_cap   = 0;      /* capacity of seq_cache          */

static bool *used = NULL;        /* used array, index is the integer value */
static size_t used_cap = 0;      /* capacity of used array          */

/* ensure seq_cache can hold at least `need` elements (0‑based). */
static void seq_grow_to(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ensure used array can index up to `need` (inclusive). */
static void used_grow_to(idx_t need)
{
    if ((idx_t)used_cap > need) return;
    size_t new_cap = used_cap ? used_cap << 1 : 16;
    while ((idx_t)new_cap <= need) new_cap <<= 1;
    used = (bool *)realloc(used, new_cap * sizeof(bool));
    /* initialize new cells to false */
    for (size_t i = used_cap; i < new_cap; ++i) used[i] = false;
    used_cap = new_cap;
}

/* mark integer `v` as used. */
static void mark_used(idx_t v)
{
    used_grow_to(v);
    used[v] = true;
}

/* return the smallest unused integer >= `start`. */
static idx_t next_unused_from(idx_t start)
{
    if (start < 1) start = 1;
    /* increase capacity if needed */
    used_grow_to(start);
    idx_t cur = start;
    while (cur < used_cap && used[cur]) {
        ++cur;
        if (cur >= used_cap) used_grow_to(cur);
    }
    return cur;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety, not used */

    /* initialise first term if needed */
    if (seq_len == 0) {
        seq_grow_to(1);
        seq_cache[0] = (seq_t)1;
        seq_len = 1;
        used_grow_to(1);
        used[1] = true;
    }

    /* build up to requested index */
    while (seq_len < n) {
        seq_t prev_val = seq_cache[seq_len - 1];
        idx_t p = (idx_t)prev_val;

        /* first interval: floor(p/3)+1 … floor(p/2) */
        idx_t low1  = p / 3 + 1;
        idx_t high1 = p / 2;

        idx_t cand1 = next_unused_from(low1);
        bool cand1_ok = (cand1 <= high1);

        /* second interval: 2p … 3p-1 */
        idx_t low2  = p * 2;
        idx_t high2 = p * 3 - 1;      /* high2 >= low2 for p>=1 */

        idx_t cand2 = next_unused_from(low2);
        bool cand2_ok = (cand2 <= high2);

        idx_t next_val;
        if (cand1_ok && cand2_ok) {
            next_val = (cand1 <= cand2) ? cand1 : cand2;
        } else if (cand1_ok) {
            next_val = cand1;
        } else if (cand2_ok) {
            next_val = cand2;
        } else {
            /* theoretically unreachable – safeguard */
            next_val = 1;
            while (true) {
                next_val = next_unused_from(next_val);
                seq_t maxv = (prev_val > (seq_t)next_val) ? prev_val : (seq_t)next_val;
                seq_t minv = (prev_val > (seq_t)next_val) ? (seq_t)next_val : prev_val;
                if (maxv / minv == 2) break;
                ++next_val;
            }
        }

        /* store and mark */
        seq_grow_to(seq_len + 1);
        seq_cache[seq_len] = (seq_t)next_val;
        mark_used(next_val);
        ++seq_len;
    }

    return seq_cache[n - 1];
}
