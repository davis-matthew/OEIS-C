/*
 * A229573  Number of defective 4-colorings of an n X 3 0..3 array connected
 *          horizontally, vertically, diagonally and antidiagonally with
 *          exactly one mistake, and colors introduced in row-major 0..3
 *          order.
 * https://oeis.org/A229573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 13*a(n-2) + 12*a(n-3) - 4*a(n-4) for n>5
 *            Initial values: a(1)=2, a(2)=26, a(3)=76, a(4)=171, a(5)=362
 *            (OEIS A229573, offset 1)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _base_vals[5] = { (seq_t)2, (seq_t)26, (seq_t)76, (seq_t)171, (seq_t)362 };
static seq_t * _cache = NULL;       /* storage for computed terms, index 1.. */
static idx_t  _cache_len = 0;       /* highest index that has been computed */
static size_t _cache_cap = 0;       /* allocated capacity (>= _cache_len+1) */

seq_t a(idx_t n)
{
    /* Handle invalid (non‑positive) index: behaviour not required.
       Return 0 for such cases. */
    if (n < 1) return (seq_t)0;

    /* Ensure storage large enough for index n */
    if (_cache_cap < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_ptr = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
        if (!new_ptr) return (seq_t)0; /* memory failure; return 0 */
        _cache = new_ptr;
        _cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t cur = _cache_len + 1; cur <= n; ++cur) {
        if (cur <= 5) {
            _cache[cur] = _base_vals[cur - 1];
        } else {
            /* Compute using only additions and subtractions to avoid
               unsigned underflow. It is guaranteed that the first sum
               is larger than the second for n > 5. */
            seq_t part1 = 6 * _cache[cur - 1];
            part1 += 12 * _cache[cur - 3];
            seq_t part2 = 13 * _cache[cur - 2];
            part2 += 4 * _cache[cur - 4];
            _cache[cur] = part1 - part2;
        }
    }
    _cache_len = n; /* update highest computed term */
    return _cache[n];
}
