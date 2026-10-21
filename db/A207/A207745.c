/*
 * A207745  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of 7×n binary arrays that avoid the horizontal patterns 0
 *            0 0 and 0 1 0, and avoid the vertical patterns 0 0 1 and 1 0 0.
 *            The function computes a(n) for n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* forward declaration of function */
seq_t a(idx_t n);

/* --------------------------------------------------------------------- */
/* Implementation details                                              */
/* --------------------------------------------------------------------- */

static int      _init_done   = 0;                /* one‑time initialisation flag       */
static uint8_t  _col[128];                      /* all valid 7‑bit columns (max 128) */
static uint8_t  _next[128*128][128];           /* list of allowed next columns      */
static uint8_t  _next_cnt[128*128];            /* number of allowed next columns    */
static size_t   _V = 0;                        /* number of valid columns            */
static size_t   _S = 0;                        /* _V * _V                            */

/* --------------------------------------------------------------------- */
/* Lazy static initialisation                                           */
/* --------------------------------------------------------------------- */
static void _init(void)
{
    if (_init_done) return;

    /* generate all 7‑bit columns that satisfy the vertical constraints   */
    size_t v = 0;
    for (int mask = 0; mask < 128; ++mask) {
        int ok = 1;
        for (int i = 0; i < 5; ++i) {                /* check triples in the column */
            int b0 = (mask >> i)     & 1;
            int b1 = (mask >> (i + 1)) & 1;
            int b2 = (mask >> (i + 2)) & 1;
            if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                (b0 == 1 && b1 == 0 && b2 == 0)) {
                ok = 0;
                break;
            }
        }
        if (ok) {
            _col[v] = (uint8_t)mask;
            ++v;
        }
    }
    _V = v;
    _S = _V * _V;

    /* pre‑compute the horizontal transitions                               */
    for (size_t i = 0; i < _V; ++i) {
        for (size_t j = 0; j < _V; ++j) {
            size_t idx = i * _V + j;
            uint8_t cnt = 0;
            for (size_t k = 0; k < _V; ++k) {
                int ok = 1;
                for (int r = 0; r < 7; ++r) {          /* check each row of the triple */
                    int b0 = (_col[i] >> r) & 1;
                    int b1 = (_col[j] >> r) & 1;
                    int b2 = (_col[k] >> r) & 1;
                    if ((b0 == 0 && b1 == 0 && b2 == 0) ||
                        (b0 == 0 && b1 == 1 && b2 == 0)) {
                        ok = 0;
                        break;
                    }
                }
                if (ok) {
                    _next[idx][cnt++] = (uint8_t)k;
                }
            }
            _next_cnt[idx] = cnt;
        }
    }

    _init_done = 1;
}

/* --------------------------------------------------------------------- */
/* DP arrays (fixed maximum size; only the first _S entries are used)     */
/* --------------------------------------------------------------------- */
#define MAXS (128 * 128)
static seq_t _dp1[MAXS];
static seq_t _dp2[MAXS];

/* --------------------------------------------------------------------- */
/* Public function                                                      */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    _init();

    /* trivial cases */
    if (n == 0) return 0;                     /* not defined for n<1, but harmless   */
    if (n == 1) {
        return (seq_t)_V;
    }
    if (n == 2) {
        seq_t v = (seq_t)_V;
        return v * v;                         /* all pairs are allowed              */
    }

    /* initialise DP for length 2: every pair of columns is allowed */
    for (size_t i = 0; i < _S; ++i) _dp1[i] = 0;
    for (size_t i = 0; i < _S; ++i) _dp1[i] = (seq_t)1;

    seq_t *cur  = _dp1;                       /* holds dp for current length        */
    seq_t *next = _dp2;                       /* buffer for next length             */

    /* iterate from length 3 up to n */
    for (uint64_t len = 3; len <= n; ++len) {
        /* zero the next array */
        for (size_t i = 0; i < _S; ++i) next[i] = 0;

        for (size_t p = 0; p < _S; ++p) {
            seq_t val = cur[p];
            if (!val) continue;               /* skip zero entries for speed        */

            size_t j = p % _V;                /* second column of the pair          */
            uint8_t cnt = _next_cnt[p];
            uint8_t *list = _next[p];
            for (uint8_t t = 0; t < cnt; ++t) {
                size_t k = list[t];
                size_t q = j * _V + k;        /* new pair (j,k)                     */
                next[q] += val;
            }
        }
        /* swap cur and next for the next iteration          */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* compute the total number of arrays of width n */
    seq_t total = 0;
    for (size_t i = 0; i < _S; ++i) total += cur[i];
    return total;
}
