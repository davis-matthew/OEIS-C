/*
 * A207447  Number of 7Xn 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of 7×n binary matrices that avoid the *
 *            patterns 0 0 1 and 0 1 0 in every row (horizontal), and avoid *
 *            0 0 1 and 1 0 0 in every column (vertical). This is computed by
 *            a * transfer‑matrix DP on the last two columns
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* --------------------------------------------------------------------
 * Precomputation of all column masks and transition lists.
 * -------------------------------------------------------------------- */
static bool _init_done = false;          /* one‑time flag */
static int  _num_masks;                  /* number of valid 7‑bit masks */
static int  _state_count;                /* = _num_masks^2 (max 1225)  */
static int  _masks[35];                  /* stored 7‑bit masks               */
static uint8_t _mask_bits[35][7];         /* bit r of mask i (0 = LSB)       */
static int   _trans_next[1225][35];      /* for every state: list of next state IDs */
static uint8_t _trans_cnt[1225];         /* number of transitions of that state */

/* --------------------------------------------------------------------
 * Initializer that builds the masks and transition tables.
 * -------------------------------------------------------------------- */
static void _init(void)
{
    if (_init_done) return;
    _init_done = true;

    /* Step 1: enumerate all 7‑bit masks that avoid the forbidden
     * vertical patterns 001 and 100.  There are 35 of them. */
    int cnt = 0;
    for (unsigned int m = 0; m < (1u << 7); ++m) {
        bool ok = true;
        for (int r = 0; r <= 4; ++r) {                /* triples in column */
            int b0 = (m >> r)     & 1;
            int b1 = (m >> (r+1)) & 1;
            int b2 = (m >> (r+2)) & 1;
            if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                (b0 == 1 && b1 == 0 && b2 == 0)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            _masks[cnt] = (int)m;
            for (int r = 0; r < 7; ++r)
                _mask_bits[cnt][r] = (uint8_t)((m >> r) & 1);
            ++cnt;
        }
    }
    _num_masks   = cnt;                    /* should be 35 */
    _state_count = _num_masks * _num_masks; /* 1225 */

    /* Step 2: build transition lists.
     * A state is a pair of masks (prev, curr).  For every such state we
     * record all possible next masks that keep horizontal patterns
     * 001 and 010 forbidden in all seven rows. */
    for (int i = 0; i < _state_count; ++i) {
        int a = i / _num_masks;            /* index of prev mask   */
        int b = i % _num_masks;            /* index of curr mask   */
        uint8_t outcnt = 0;

        for (int c = 0; c < _num_masks; ++c) {          /* candidate next mask */
            bool ok = true;
            for (int r = 0; r < 7; ++r) {
                int p = _mask_bits[a][r];
                int q = _mask_bits[b][r];
                int r3 = _mask_bits[c][r];
                if ((p == 0 && q == 0 && r3 == 1) ||
                    (p == 0 && q == 1 && r3 == 0)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                int ns = b * _num_masks + c;             /* next state id */
                _trans_next[i][outcnt++] = ns;
            }
        }
        _trans_cnt[i] = outcnt;
    }
}

/* --------------------------------------------------------------------
 * Sequence computation.
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    _init();                               /* ensure tables built */

    if (n == 1) return (seq_t)35;          /* only one column possible */
    if (n == 2) return (seq_t)(35 * 35);    /* all pairs of valid columns */

    /* DP arrays for the current and next step.  Size 35^2 = 1225. */
    static seq_t dp[1225];
    static seq_t ndp[1225];
    int  stcnt = _state_count;             /* 1225 */

    /* initialise dp for all (mask1,mask2) with count 1 */
    for (int i = 0; i < stcnt; ++i) dp[i] = (seq_t)1;

    /* iterate from the 3rd column up to column n */
    for (idx_t step = 3; step <= n; ++step) {
        /* zero ndp */
        for (int i = 0; i < stcnt; ++i) ndp[i] = 0;

        /* spread counts via transitions */
        for (int s = 0; s < stcnt; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            uint8_t  tcnt   = _trans_cnt[s];
            int *    tlist  = _trans_next[s];
            for (uint8_t k = 0; k < tcnt; ++k) {
                int ns = tlist[k];
                ndp[ns] += val;
            }
        }

        /* swap dp and ndp for next iteration */
        for (int i = 0; i < stcnt; ++i) {
            dp[i] = ndp[i];
        }
    }

    /* sum all dp values: any final state is allowed */
    seq_t ans = 0;
    for (int i = 0; i < stcnt; ++i) ans += dp[i];
    return ans;
}
