/*
 * A169674  Lexicographically earliest de Bruijn sequence for n = 8 and k = 2.
 * https://oeis.org/A169674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1023, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest binary de Bruijn sequence for n=8 (period = 256). */
/* Initialized on first call; a(idx_t n) returns the bit at position n (mod 256). */
#include <stddef.h>

#ifndef _DEBRUIJN_INIT
#define _DEBRUIJN_INIT
static seq_t _debruijn_seq[256];
static int _debruijn_len = 0;
static int _debruijn_done = 0;

/* Recursive generation of de Bruijn sequence for alphabet size k and length n.
 * Classic algorithm by Fredricksen–Maiorana.  a[0] must be initialized to 0. */
static void _db_rec(int t, int p, int n, int k, int *a)
{
    if (t > n) {
        if ((n % p) == 0) {
            for (int i = 1; i <= p; ++i) {
                _debruijn_seq[_debruijn_len++] = (seq_t)a[i];
            }
        }
    } else {
        a[t] = a[t - p];
        _db_rec(t + 1, p, n, k, a);
        for (int j = a[t - p] + 1; j < k; ++j) {
            a[t] = j;
            _db_rec(t + 1, t, n, k, a);
        }
    }
}

/* Initialise the static de Bruijn array on first use. */
static void _init_debruijn(void)
{
    if (_debruijn_done) return;
    const int n = 8;
    const int k = 2;
    int a[9];          /* 1‑based indexing; a[0] is used as the initial zero. */
    a[0] = 0;          /* crucial initialisation */
    _db_rec(1, 1, n, k, a);
    _debruijn_done = 1;
}
#endif

seq_t a(idx_t n)
{
    _init_debruijn();
    return _debruijn_seq[(size_t)(n & 255)];
}
