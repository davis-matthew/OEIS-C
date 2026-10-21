/*
 * A214642  Indices of b in the sequence a -> abc, b -> ab, c -> b . Start
 *          with a.
 * https://oeis.org/A214642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A214642 – indices of 'b' in the infinite word produced by
   a → abc, b → ab, c → b (morphism).  The function returns the
   1‑based position of the n-th 'b' in that word. */
#include <stdint.h>

#define NUM_SYMS 3               /* a=0, b=1, c=2 */
#define MAX_DEPTH 64             /* depth large enough for requested n */

/* Children of each symbol (indices into NUM_SYMS). Extra slots are unused. */
static const int child_list[NUM_SYMS][3] = {
    {0, 1, 2},   /* a → abc */
    {0, 1, 0},   /* b → ab  */
    {1, 0, 0}    /* c → b   */
};
static const int child_len[NUM_SYMS] = {3, 2, 1};

/* Memoised lengths and b‑counts for each symbol at each depth */
static seq_t _len[NUM_SYMS][MAX_DEPTH + 1];
static seq_t _bcnt[NUM_SYMS][MAX_DEPTH + 1];
static int   _max_depth_computed = -1;   /* highest depth that has been computed */

/* Recursive helper: offset of the t‑th 'b' within the expansion of `sym`
   at `depth`.  `acc` holds the total length of symbols already skipped. */
static seq_t _find_offset(int sym, int depth, idx_t t,
                          const seq_t (*bcnt)[MAX_DEPTH + 1],
                          const seq_t (*len )[MAX_DEPTH + 1],
                          seq_t acc)
{
    if (depth == 0) {                /* leaf symbol */
        /* The leaf must be 'b', otherwise this branch would never be taken. */
        return acc;
    }

    for (int ci = 0; ci < child_len[sym]; ++ci) {
        int child = child_list[sym][ci];
        seq_t cnt = bcnt[child][depth - 1];

        if (cnt >= (seq_t)t) {                     /* the desired 'b' is in this child */
            return _find_offset(child, depth - 1, t, bcnt, len, acc);
        }

        /* skip the whole child */
        t   -= (idx_t)cnt;
        acc += len[child][depth - 1];
    }
    /* logically unreachable – the input guarantee ensures enough 'b's. */
    return acc;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Index 0 is outside the OEIS offset */

    /* Initialise depth 0 on first call */
    if (_max_depth_computed == -1) {
        for (int s = 0; s < NUM_SYMS; ++s) {
            _len[s][0]  = 1;
            _bcnt[s][0] = (s == 1) ? 1 : 0;   /* only 'b' contributes a count */
        }
        _max_depth_computed = 0;
    }

    /* Compute deeper depths until we have enough 'b's in 'a' */
    while (_bcnt[0][_max_depth_computed] < n && _max_depth_computed < MAX_DEPTH - 1) {
        int d = _max_depth_computed + 1;
        for (int s = 0; s < NUM_SYMS; ++s) {
            seq_t len_sum = 0;
            seq_t cnt_sum = 0;
            for (int ci = 0; ci < child_len[s]; ++ci) {
                int child = child_list[s][ci];
                len_sum += _len[child][d - 1];
                cnt_sum += _bcnt[child][d - 1];
            }
            _len[s][d]  = len_sum;
            _bcnt[s][d] = cnt_sum;
        }
        _max_depth_computed = d;
    }

    /* If still not enough (extremely large n), return 0 as safe fallback. */
    if (_bcnt[0][_max_depth_computed] < n) return 0;

    /* Find 0‑based offset of nth 'b' in the expansion of 'a' at depth d,
       then convert to 1‑based position. */
    seq_t offset = _find_offset(0, _max_depth_computed, n,
                                _bcnt, _len, 0);
    return offset + (seq_t)1;
}
