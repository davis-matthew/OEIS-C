/*
 * A207307  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×n binary matrices avoiding horizontally the
 *            patterns 0 0 0 and 1 0 1, and vertically the patterns 0 0 1 and
 *            1 1 0. Computation uses a transfer‑matrix DP with a state
 *            consisting of the last two columns (each 4 bits). The recurrence
 *            is * a(1) = 8 * a(2) = 64 * for n ≥ 3: a(n) = Σ...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 40, u128 n <= 82,
 *            big any n
 * Helpers:   _a_sdp_transition_ok, _a_sdp_vertical_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_a_sdp_transition_ok.h"
#include "../utils/_a_sdp_vertical_valid.h"

/* Static helper data for the transfer matrix (computed once). */
static int          _a_sdp_initialized = 0;
static unsigned char _a_sdp_valid[16];
static uint16_t     _a_sdp_allowed[256];
static seq_t        _a_sdp_dp_prev[256];
static seq_t        _a_sdp_dp_next[256];

/* Lazy initialisation of helper data. */
static void
_a_sdp_init(void)
{
    if (_a_sdp_initialized) return;
    _a_sdp_initialized = 1;

    /* Valid single columns. */
    for (int s = 0; s < 16; ++s)
        _a_sdp_valid[s] = _a_sdp_vertical_valid((unsigned char)s);

    /* Allowed transitions for each pair of previous columns. */
    for (int a = 0; a < 16; ++a)
        for (int b = 0; b < 16; ++b)
            for (int c = 0; c < 16; ++c) {
                if (_a_sdp_valid[a] && _a_sdp_valid[b] && _a_sdp_valid[c] &&
                    _a_sdp_transition_ok((unsigned char)a,
                                         (unsigned char)b,
                                         (unsigned char)c))
                    _a_sdp_allowed[(a << 4) | b] |= (uint16_t)(1 << c);
            }
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, return 0 */

    _a_sdp_init();

    /* Count of vertically valid columns. */
    seq_t valid_cnt = 0;
    for (int s = 0; s < 16; ++s)
        if (_a_sdp_valid[s]) ++valid_cnt;

    if (n == 1) return valid_cnt;
    if (n == 2) return valid_cnt * valid_cnt;  /* no horizontal patterns yet */

    /* Initialise DP for n = 2: all pairs of valid columns have count 1. */
    for (int i = 0; i < 256; ++i) _a_sdp_dp_prev[i] = 0;
    for (int a = 0; a < 16; ++a)
        if (_a_sdp_valid[a])
            for (int b = 0; b < 16; ++b)
                if (_a_sdp_valid[b])
                    _a_sdp_dp_prev[(a << 4) | b] += 1;

    /* DP over additional columns (from 3 to n). */
    for (idx_t col = 3; col <= n; ++col) {
        /* Zero next array. */
        for (int i = 0; i < 256; ++i) _a_sdp_dp_next[i] = 0;

        for (int idx = 0; idx < 256; ++idx) {
            seq_t val = _a_sdp_dp_prev[idx];
            if (!val) continue;
            int prev1 = idx & 15;        /* second previous column */
            uint16_t mask = _a_sdp_allowed[idx];
            for (int new_col = 0; new_col < 16; ++new_col) {
                if (!(mask & (1 << new_col))) continue;
                int new_idx = (prev1 << 4) | new_col;
                _a_sdp_dp_next[new_idx] += val;
            }
        }
        /* Swap dp arrays. */
        for (int i = 0; i < 256; ++i) _a_sdp_dp_prev[i] = _a_sdp_dp_next[i];
    }

    /* Sum over all ending states to get the total count. */
    seq_t total = 0;
    for (int i = 0; i < 256; ++i) total += _a_sdp_dp_prev[i];
    return total;
}
