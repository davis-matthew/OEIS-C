/*
 * A230901  Sturmian word: equals the limit word S(infinity) where S(0) = 0,
 *          S(1) = 1 and for n >= 1, S(n+1) = S(n)S(n)S(n)S(n-1).
 * https://oeis.org/A230901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+2)/α) - floor((n+1)/α); α = (√13‑1)/2.
 *            Implementation generates the limit word S(∞) by the recurrence
 *            S(n+1) = S(n) S(n) S(n) S(n–1) and returns the n‑th character
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>  /* for size_t */

/* Buffers large enough for n up to about 10 000 (word length ≈ 46 000). */
#define MAX_WORD_LEN 60000

static seq_t _S_cur[MAX_WORD_LEN];
static idx_t _S_cur_len = 0;
static seq_t _S_prev[MAX_WORD_LEN];
static idx_t _S_prev_len = 0;
static bool  _S_init = false;

seq_t a(idx_t n)
{
    /* initialise S(1)=1, S(0)=0 */
    if (!_S_init) {
        _S_cur[0]     = 1;
        _S_cur_len    = 1;
        _S_prev[0]    = 0;
        _S_prev_len   = 1;
        _S_init       = true;
    }

    /* grow the word until the required index is available */
    while (_S_cur_len <= n) {
        idx_t new_len = 3 * _S_cur_len + _S_prev_len;
        static seq_t _tmp[MAX_WORD_LEN];

        /* copy three copies of the current word */
        for (idx_t i = 0; i < _S_cur_len; ++i)        _tmp[i] = _S_cur[i];
        for (idx_t i = 0; i < _S_cur_len; ++i)        _tmp[_S_cur_len + i] = _S_cur[i];
        for (idx_t i = 0; i < _S_cur_len; ++i)        _tmp[2 * _S_cur_len + i] = _S_cur[i];

        /* copy the previous word once */
        for (idx_t i = 0; i < _S_prev_len; ++i)
            _tmp[3 * _S_cur_len + i] = _S_prev[i];

        /* update state: old current becomes new previous */
        for (idx_t i = 0; i < _S_cur_len; ++i)
            _S_prev[i] = _S_cur[i];
        _S_prev_len = _S_cur_len;

        /* new word becomes the current word */
        _S_cur_len = new_len;
        for (idx_t i = 0; i < new_len; ++i)
            _S_cur[i] = _tmp[i];
    }

    return _S_cur[n];
}
