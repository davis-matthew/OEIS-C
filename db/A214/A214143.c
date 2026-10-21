/*
 * A214143  Number of 0..4 colorings of a 2 X (n+1) array circular in the n+1
 *          direction with new values 0..4 introduced in row major order.
 * https://oeis.org/A214143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) + 50*a(n-2) - 116*a(n-3) - 361*a(n-4) +
 *            106*a(n-5) + 312*a(n-6), a(1)=4, a(2)=17, a(3)=257, a(4)=3074,
 *            a(5)=40434, a(6)=522515
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t _max_cached = 0;
static seq_t * _cache = NULL;

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, but return 0 */

    /* Allocate or grow cache if necessary */
    if (_max_cached < n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(_cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed – return 0 */
        _cache = tmp;

        /* Initialize base values if not already present */
        for (idx_t i = _max_cached + 1; i <= n; ++i) {
            if (i == 1) _cache[1] = (seq_t)4;
            else if (i == 2) _cache[2] = (seq_t)17;
            else if (i == 3) _cache[3] = (seq_t)257;
            else if (i == 4) _cache[4] = (seq_t)3074;
            else if (i == 5) _cache[5] = (seq_t)40434;
            else if (i == 6) _cache[6] = (seq_t)522515;
            else {
                /* recurrence: a(i) = 10*a(i-1) + 50*a(i-2) - 116*a(i-3)
                   - 361*a(i-4) + 106*a(i-5) + 312*a(i-6) */
                _cache[i] = _cache[i-1] * (seq_t)10
                          + _cache[i-2] * (seq_t)50
                          - _cache[i-3] * (seq_t)116
                          - _cache[i-4] * (seq_t)361
                          + _cache[i-5] * (seq_t)106
                          + _cache[i-6] * (seq_t)312;
            }
        }
        _max_cached = n;
    }

    return _cache[n];
}
