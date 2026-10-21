/*
 * A063815  Growth series for fundamental group of orientable closed surface
 *          of genus 5.
 * https://oeis.org/A063815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1) - 19*a(n-10) + a(n-11) a(0) = 1, a(1) = 20,
 *            a(2) = 380, a(3) = 7220, a(4) = 137180, a(5) = 2606420, a(6) =
 *            49521980, a(7) = 940917620, a(8) = 17877434780, a(9) =
 *            339671260820, a(10) = 6453753955560, a(11) = 122621325155260
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init[12] = {
    (seq_t)1ULL,
    (seq_t)20ULL,
    (seq_t)380ULL,
    (seq_t)7220ULL,
    (seq_t)137180ULL,
    (seq_t)2606420ULL,
    (seq_t)49521980ULL,
    (seq_t)940917620ULL,
    (seq_t)17877434780ULL,
    (seq_t)339671260820ULL,
    (seq_t)6453753955560ULL,
    (seq_t)122621325155260ULL
};

static seq_t *_memo = NULL;        /* dynamically allocated cache */
static size_t _memo_len = 0;       /* number of terms stored (>=1 if non‑NULL) */
static size_t _memo_cap = 0;       /* current allocation capacity */

static void _ensure_capacity(size_t needed)
{
    if (needed <= _memo_cap)
        return;
    size_t new_cap = (_memo_cap == 0) ? 12 : _memo_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(_memo, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* allocation failure – return zero as safe fallback */
        return;
    }
    _memo = new_ptr;
    _memo_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* convert idx_t to a usable size_t – safe for typical usage */
    size_t idx = (size_t)n;

    /* initialise cache if empty */
    if (!_memo) {
        _ensure_capacity(12);
        while (_memo_cap > 0 && _memo_len < 12) {
            _memo[_memo_len++] = init[_memo_len];
        }
    }

    /* make sure we have room for idx */
    if (idx >= _memo_len) {
        _ensure_capacity(idx + 1);
        for (size_t i = _memo_len; i <= idx; ++i) {
            if (i < 12) {
                _memo[i] = init[i];
            } else {
                _memo[i] = (seq_t)19 * _memo[i-1]
                          - (seq_t)19 * _memo[i-10]
                          + _memo[i-11];
            }
        }
        _memo_len = idx + 1;
    }

    return _memo[idx];
}
