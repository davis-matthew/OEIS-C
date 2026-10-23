/*
 * A045017  Numbers whose base-4 representation contains no 0's and exactly
 *          four 1's.
 * https://oeis.org/A045017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base-4 representation contains no 0's and exactly
 *            four 1's
 * Range:     u8 n <= 1, u16 n <= 1471, u32 n <= 2700, u64 n <= 2700,
 *            u128 n <= 2700, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool _binom_ready = false;
static idx_t _binom[65][65];

static void _init_binom(void)
{
    if (_binom_ready) return;
    for (int n = 0; n <= 64; ++n) {
        _binom[n][0] = 1;
        _binom[n][n] = 1;
        for (int k = 1; k < n; ++k) {
            _binom[n][k] = _binom[n-1][k-1] + _binom[n-1][k];
        }
    }
    _binom_ready = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, safety only */
    _init_binom();

    idx_t cum = 0;
    int L = 4;                      /* minimal length with 4 ones */
    for (;;) {
        idx_t cnt = _binom[L][4] << (L - 4);   /* C(L,4) * 2^(L-4) */
        if (cum + cnt >= n) break;
        cum += cnt;
        ++L;
    }

    idx_t k = n - cum;              /* 1‑based index inside length L */
    idx_t value = 0;
    int remOnes = 4;

    for (int pos = 0; pos < L; ++pos) {
        int remPos = L - pos - 1;

        /* try digit 1 */
        idx_t cnt1 = 0;
        if (remOnes > 0) {
            cnt1 = _binom[remPos][remOnes-1] << (remPos - (remOnes-1));
            if (k <= cnt1) {
                value = value * 4 + 1;
                --remOnes;
                continue;
            } else {
                k -= cnt1;
            }
        }

        /* try digit 2 */
        idx_t cnt23 = 0;
        if (remOnes <= remPos) {
            cnt23 = _binom[remPos][remOnes] << (remPos - remOnes);
            if (k <= cnt23) {
                value = value * 4 + 2;
                continue;
            } else {
                k -= cnt23;
            }
        }

        /* digit 3 is the only remaining option */
        value = value * 4 + 3;
    }
    return (seq_t)value;
}
