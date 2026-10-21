/*
 * A202403  Number of (n+2) X 7 binary arrays avoiding patterns 000 and 010 in
 *          rows and columns.
 * https://oeis.org/A202403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 27,
 *            big any n
 * Helpers:   _is_valid_row, _vertical_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A202403: Number of (n+2)×7 binary arrays avoiding 000 and 010
 *  in rows and columns.
 *
 *  The code builds all binary words of length 7 that themselves avoid
 *  000 and 010.  Each such word is a valid row.  It then pre‑computes,
 *  for every pair of consecutive rows (a,b), the list of possible
 *  third rows c so that column triples (x,y,z) never contain 000 or
 *  010.  The DP state is the last two rows; the transition from (a,b)
 *  to (b,c) is allowed exactly when the vertical constraint holds.
 *
 *  For a given n the number of rows is N = n + 2.  We initialise the
 *  DP with all possible first two rows (each starting weight 1), and
 *  iterate over the remaining N‑2 rows using the pre‑computed
 *  transitions.  The sum of all ending states equals the desired
 *  count.  The algorithm is O(M² + M²⋅n) where M is the number of
 *  valid row patterns (M≈60), easily satisfying the time limits up
 *  to n≈10 000.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_is_valid_row.h"
#include "../utils/_vertical_ok.h"

/* ---------- static tables for the DP, built once ---------- */
static bool _a202403_inited = false;
static int  _A202403_M;                  /* number of valid rows            */
static uint8_t *_A202403_rows;           /* list of valid rows (word)       */
static size_t _A202403_S;                /* state count = M*M                */
static int **_A202403_next;              /* transitions per state           */
static int  *_A202403_next_cnt;          /* number of outgoing edges per state */

/* ------------------------------------------------------------ */
/*  One‑time initialization: build rows and transition tables   */
static void _a202403_init(void)
{
    /* gather all valid rows */
    _A202403_M = 0;
    for (int w = 0; w < 128; ++w)
        if (_is_valid_row((uint8_t)w))
            ++_A202403_M;

    _A202403_rows = (uint8_t*)malloc(_A202403_M * sizeof(uint8_t));
    int idx = 0;
    for (int w = 0; w < 128; ++w)
        if (_is_valid_row((uint8_t)w))
            _A202403_rows[idx++] = (uint8_t)w;

    /* state count */
    _A202403_S = (size_t)_A202403_M * (size_t)_A202403_M;

    /* allocate transition structures */
    _A202403_next     = (int**)malloc(_A202403_S * sizeof(int*));
    _A202403_next_cnt = (int*)  malloc(_A202403_S * sizeof(int));

    for (size_t s = 0; s < _A202403_S; ++s) {
        _A202403_next[s]     = NULL;
        _A202403_next_cnt[s] = 0;
    }

    /* build transitions */
    for (int a = 0; a < _A202403_M; ++a) {
        for (int b = 0; b < _A202403_M; ++b) {
            size_t st = (size_t)a * (size_t)_A202403_M + (size_t)b;
            int cnt = 0;
            for (int c = 0; c < _A202403_M; ++c)
                if (_vertical_ok(_A202403_rows[a],
                                 _A202403_rows[b],
                                 _A202403_rows[c]))
                    ++cnt;
            _A202403_next_cnt[st] = cnt;
            if (cnt == 0) continue;
            int *arr = (int*)malloc(cnt * sizeof(int));
            int pos = 0;
            for (int c = 0; c < _A202403_M; ++c)
                if (_vertical_ok(_A202403_rows[a],
                                 _A202403_rows[b],
                                 _A202403_rows[c]))
                    arr[pos++] = c;
            _A202403_next[st] = arr;
        }
    }

    _a202403_inited = true;
}

/* ------------------------------------------------------------ */
/*  Sequence function required by the build                      */
seq_t a(idx_t n)
{
    if (!_a202403_inited) _a202403_init();

    /* number of rows in the array */
    idx_t Nrows = n + (idx_t)2;
    if (Nrows < 2) return (seq_t)0;   /* should not happen for n>=1 */

    /* DP arrays: one per state */
    size_t states = _A202403_S;
    seq_t *cur = (seq_t*)calloc(states, sizeof(seq_t));
    seq_t *nxt = (seq_t*)calloc(states, sizeof(seq_t));
    if (!cur || !nxt) { free(cur); free(nxt); return (seq_t)0; }

    /* initialise with all pairs of first two rows */
    for (size_t st = 0; st < states; ++st) cur[st] = (seq_t)1;

    /* fill remaining rows */
    for (idx_t step = 2; step <= Nrows - 1; ++step) {
        for (size_t i = 0; i < states; ++i) nxt[i] = (seq_t)0;

        for (size_t st = 0; st < states; ++st) {
            seq_t val = cur[st];
            if (!val) continue;
            int cnt = _A202403_next_cnt[st];
            int *list = _A202403_next[st];
            int a = (int)(st / (size_t)_A202403_M);
            int b = (int)(st % (size_t)_A202403_M);
            for (int i = 0; i < cnt; ++i) {
                int c = list[i];
                size_t nxt_state = (size_t)b * (size_t)_A202403_M + (size_t)c;
                nxt[nxt_state] += val;
            }
        }
        /* swap cur and nxt */
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
    }

    /* sum over all final states */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i < states; ++i) total += cur[i];

    free(cur);
    free(nxt);
    return total;
}
