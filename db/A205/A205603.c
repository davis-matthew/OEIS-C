/*
 * A205603  Number of (n+1)X3 0..3 arrays with every 2X2 subblock having the
 *          same number of equal edges as its horizontal neighbors and a
 *          different number from its vertical neighbors, and new values 0..3
 *          introduced in row major order.
 * https://oeis.org/A205603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 192*a(n-2) + 1024*a(n-3) - 994*a(n-4) - 29144*a(n-5) * -
 *            57188*a(n-6) + 259824*a(n-7) + 773391*a(n-8) * - 743544*a(n-9) -
 *            3411360*a(n-10) - 399168*a(n-11) * + 4852224*a(n-12) +
 *            2985984*a(n-13) * Initial values (a(1) .. a(14)): * 77, 1030,
 *            15896, 248392, 3889612, 60868362, 953231848, * 1492...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t _a205603_init[15] = {
    0ULL,
    77ULL, 1030ULL, 15896ULL, 248392ULL, 3889612ULL,
    60868362ULL, 953231848ULL, 14922106020ULL,
    233649216140ULL, 3657937755750ULL,
    57272812041768ULL, 896673748420488ULL,
    14039041888152956ULL, 219800897926415930ULL
};

static seq_t * _a205603_cache = NULL;
static size_t  _a205603_cache_len = 0;   /* allocated length */
static idx_t   _a205603_max_computed = 0;/* highest n whose value is stored */

seq_t a(idx_t n)
{
    /* Return known values directly for the first 14 terms */
    if (n <= 14)
        return _a205603_init[(size_t)n];

    /* Initialise cache on first use */
    if (_a205603_cache == NULL) {
        _a205603_cache_len = 15;
        _a205603_cache = (seq_t *)malloc(_a205603_cache_len * sizeof(seq_t));
        for (idx_t i = 1; i <= 14; ++i)
            _a205603_cache[i] = _a205603_init[(size_t)i];
        _a205603_cache[0] = 0;
        _a205603_max_computed = 14;
    }

    /* Grow cache to hold up to n */
    if ((size_t)(n + 1) > _a205603_cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(_a205603_cache, new_len * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failed, return 0 as fallback */
            return 0;
        }
        _a205603_cache = tmp;
        _a205603_cache_len = new_len;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = _a205603_max_computed + 1; i <= n; ++i) {
        seq_t pos = _a205603_cache[i - 2] * 192ULL;
        pos += _a205603_cache[i - 3] * 1024ULL;
        pos += _a205603_cache[i - 7] * 259824ULL;
        pos += _a205603_cache[i - 8] * 773391ULL;
        pos += _a205603_cache[i - 12] * 4852224ULL;
        pos += _a205603_cache[i - 13] * 2985984ULL;

        seq_t neg = _a205603_cache[i - 4] * 994ULL;
        neg += _a205603_cache[i - 5] * 29144ULL;
        neg += _a205603_cache[i - 6] * 57188ULL;
        neg += _a205603_cache[i - 9] * 743544ULL;
        neg += _a205603_cache[i - 10] * 3411360ULL;
        neg += _a205603_cache[i - 11] * 399168ULL;

        _a205603_cache[i] = pos - neg;
    }
    _a205603_max_computed = n;

    return _a205603_cache[n];
}
