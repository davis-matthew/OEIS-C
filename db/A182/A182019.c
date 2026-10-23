/*
 * A182019  Number of independent sets of nodes in graph C_8 X P_n (n>=0).
 * https://oeis.org/A182019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29*a(n-1) - 65*a(n-2) - 317*a(n-3) + 334*a(n-4) * +
 *            187*a(n-5) - 109*a(n-6) + 5*a(n-7) + a(n-8); * Initial values: *
 *            a(0)=1, a(1)=47, a(2)=1155, a(3)=30277, a(4)=788453, *
 *            a(5)=20546803, a(6)=535404487, a(7)=13951571713
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _A_init[8] = {
    (seq_t)1,
    (seq_t)47,
    (seq_t)1155,
    (seq_t)30277,
    (seq_t)788453,
    (seq_t)20546803,
    (seq_t)535404487,
    (seq_t)13951571713u
};

static seq_t * _A_cache = NULL;      /* dynamic cache of computed values     */
static idx_t   _A_cache_size = 0;    /* allocated number of slots           */
static idx_t   _A_last_computed = 7; /* highest index already stored in cache */

static void _A_ensure_cache(idx_t n)
{
    /* Allocate or grow the cache if needed. */
    if (_A_cache == NULL) {
        _A_cache_size = (n + 1 > 8) ? (n + 1) : 8;
        _A_cache = (seq_t*)malloc(_A_cache_size * sizeof(seq_t));
        if (_A_cache == NULL) return;          /* out of memory – unlikely in tests */
        for (idx_t i = 0; i < 8; ++i)
            _A_cache[i] = _A_init[i];
        _A_last_computed = 7;
    } else if (_A_cache_size <= n) {
        size_t new_size = _A_cache_size * 2;
        if ((size_t)n + 1 > new_size)
            new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(_A_cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return;              /* out of memory – unlikely */
        _A_cache = tmp;
        _A_cache_size = new_size;
    }

    /* Compute any missing terms up to index n. */
    if (_A_last_computed < n) {
        for (idx_t i = _A_last_computed + 1; i <= n; ++i) {
            seq_t pos = (seq_t)29  * _A_cache[i-1];
            pos += (seq_t)334 * _A_cache[i-4];
            pos += (seq_t)187 * _A_cache[i-5];
            pos += (seq_t)5   * _A_cache[i-7];
            pos +=              _A_cache[i-8];

            seq_t neg = (seq_t)65  * _A_cache[i-2];
            neg += (seq_t)317 * _A_cache[i-3];
            neg += (seq_t)109 * _A_cache[i-6];

            _A_cache[i] = pos - neg;
        }
        _A_last_computed = n;
    }
}

seq_t a(idx_t n)
{
    if (n < 8)
        return _A_init[n];

    _A_ensure_cache(n);
    return _A_cache[n];
}
