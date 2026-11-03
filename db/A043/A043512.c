/*
 * A043512  Numbers having four 5's in base 10.
 * https://oeis.org/A043512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 41, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 47 terms through n = 8779 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A043512: Numbers having exactly four 5's in base 10.
    The function generates the n‑th such number by enumerating
    all k‑digit numbers (k ≥ 4) in ascending order until the
    desired index is reached. */

#include <stdbool.h>

static void _enum_rec(idx_t pos, idx_t k, seq_t pref,
                      idx_t rem_fives, idx_t *cnt,
                      idx_t target, seq_t *res)
{
    if (*cnt >= target)
        return;
    if (pos == k) {
        if (rem_fives == 0) {
            (*cnt)++;
            if (*cnt == target)
                *res = pref;
        }
        return;
    }
    int start = (pos == 0) ? 1 : 0;   /* no leading zero */
    for (int d = start; d <= 9; ++d) {
        idx_t new_rem = rem_fives - (d == 5);
        if (new_rem < 0)
            continue;
        idx_t remaining = k - pos - 1;
        if (new_rem > remaining)
            continue;
        seq_t new_pref = pref * (seq_t)10 + (seq_t)d;
        _enum_rec(pos + 1, k, new_pref, new_rem, cnt, target, res);
        if (*cnt >= target)
            return;
    }
}

static void _enumerate_k(idx_t k, idx_t *cnt,
                         idx_t target, seq_t *res)
{
    _enum_rec(0, k, (seq_t)0, (idx_t)4, cnt, target, res);
}

seq_t a(idx_t n)
{
    if (n == 0)        /* the sequence is 1‑based; undefined for 0 */
        return 0;
    idx_t count = 0;
    seq_t result = 0;
    for (idx_t k = 4; ; ++k) {
        _enumerate_k(k, &count, n, &result);
        if (count >= n)
            return result;
    }
}
