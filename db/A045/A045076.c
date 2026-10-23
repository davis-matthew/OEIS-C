/*
 * A045076  Numbers whose base-4 representation contains exactly three 0's and
 *          no 3's.
 * https://oeis.org/A045076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number whose base‑4 representation contains exactly three
 *            0’s and no 3’s
 * Range:     u8 n <= 2, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* pre‑compute cumulative counts of admissible numbers up to length 20
   (enough for all n that fit comfortably in idx_t). */
static idx_t _len_cum[21];
static bool _len_cum_init = false;

static seq_t _choose(int n, int k)
{
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (int i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

static seq_t _pow2(int e)
{
    return ((seq_t)1) << e;
}

static void _init_len_cum(void)
{
    _len_cum[0] = 0;
    for (int L = 1; L <= 20; ++L) {
        seq_t cnt = 0;
        if (L >= 4) {
            seq_t c  = _choose(L - 1, 3);
            seq_t p  = _pow2(L - 3);
            cnt = c * p;                 /* total numbers of length L */
        }
        _len_cum[L] = _len_cum[L - 1] + (idx_t)cnt;
    }
    _len_cum_init = true;
}

/* number of admissible strings with 'rem_pos' remaining positions
   when we still need 'zeros_needed' zeros. */
static idx_t _count_strings(int rem_pos, int zeros_needed)
{
    if (zeros_needed < 0 || zeros_needed > rem_pos) return 0;
    seq_t ways = _choose(rem_pos, zeros_needed) * _pow2(rem_pos - zeros_needed);
    return (idx_t)ways;
}

seq_t a(idx_t n)
{
    if (!_len_cum_init) _init_len_cum();

    /* find the length L such that cumulative[L-1] < n <= cumulative[L] */
    int L;
    for (L = 1; L <= 20; ++L)
        if (_len_cum[L] >= n) break;

    /* offset within the block of length L numbers (0‑based) */
    idx_t offset = n - _len_cum[L - 1] - 1;

    int zeros_needed = 3;
    seq_t res = 0;

    for (int pos = 1; pos <= L; ++pos) {
        if (pos == 1) {            /* first digit: 1 or 2 */
            for (int d = 1; d <= 2; ++d) {
                int zeros_left = zeros_needed - (d == 0 ? 1 : 0);
                int rem_pos   = L - pos;
                idx_t cnt     = _count_strings(rem_pos, zeros_left);
                if (offset < cnt) {
                    res = res * 4 + (seq_t)d;
                    zeros_needed = zeros_left;
                    break;
                }
                offset -= cnt;
            }
        } else {                    /* remaining digits: 0,1,2 */
            for (int d = 0; d <= 2; ++d) {
                int zeros_left = zeros_needed - (d == 0 ? 1 : 0);
                int rem_pos   = L - pos;
                idx_t cnt     = _count_strings(rem_pos, zeros_left);
                if (offset < cnt) {
                    res = res * 4 + (seq_t)d;
                    zeros_needed = zeros_left;
                    break;
                }
                offset -= cnt;
            }
        }
    }
    return res;
}
