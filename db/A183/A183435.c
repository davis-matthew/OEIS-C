/*
 * A183435  Number of n X 2 binary arrays with every 1 having exactly one
 *          king-move neighbor equal to 1.
 * https://oeis.org/A183435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 4*a(n-3); with a(1)=2, a(2)=7, a(3)=13
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 54, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
static idx_t _last_computed = 0;
static std::vector<seq_t> _cache(1);          // _cache[0] unused
seq_t a(idx_t n) {
    if (n <= _last_computed)                 // already computed
        return _cache[n];
    if (_cache.size() <= n)                  // resize to fit n
        _cache.resize(n + 1);
    for (idx_t i = _last_computed + 1; i <= n; ++i) {
        if (i == 1)
            _cache[1] = (seq_t)2;
        else if (i == 2)
            _cache[2] = (seq_t)7;
        else if (i == 3)
            _cache[3] = (seq_t)13;
        else
            _cache[i] = _cache[i - 1] + _cache[i - 2] + (seq_t)4 * _cache[i - 3];
    }
    _last_computed = n;
    return _cache[n];
}
#else
#include <stdlib.h>
static seq_t _cache[10001] = {0};              // up to n = 10000
static idx_t _last_computed = 0;
seq_t a(idx_t n) {
    if (n <= _last_computed)                  // already computed
        return _cache[n];
    for (idx_t i = _last_computed + 1; i <= n; ++i) {
        if (i == 1)
            _cache[1] = (seq_t)2;
        else if (i == 2)
            _cache[2] = (seq_t)7;
        else if (i == 3)
            _cache[3] = (seq_t)13;
        else
            _cache[i] = _cache[i - 1] + _cache[i - 2] + (seq_t)4 * _cache[i - 3];
    }
    _last_computed = n;
    return _cache[n];
}
#endif
