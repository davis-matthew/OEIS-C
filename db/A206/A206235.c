/*
 * A206235  Number of (n+1)X6 0..3 arrays with every 2X3 or 3X2 subblock
 *          having exactly two equal edges, and new values 0..3 introduced in
 *          row major order.
 * https://oeis.org/A206235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 921*a(n-3) - 921*a(n-4) for n>9 Initial terms:
 *            a(1)=8910, a(2)=31652, a(3)=133392, a(4)=1236432, a(5)=10915392,
 *            a(6)=118573968, a(7)=1122086640, a(8)=10022726928,
 *            a(9)=109206613488 (n>=1)
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   mul921
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul921.h"

static const seq_t init_vals[10] = {
    0ULL,
    (seq_t)8910ULL,
    (seq_t)31652ULL,
    (seq_t)133392ULL,
    (seq_t)1236432ULL,
    (seq_t)10915392ULL,
    (seq_t)118573968ULL,
    (seq_t)1122086640ULL,
    (seq_t)10022726928ULL,
    (seq_t)109206613488ULL
};

static seq_t *cache = NULL;
static idx_t cached_n = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if (n <= 9)
        return init_vals[n];

    if (cache == NULL) {
        cache = (seq_t *)calloc(10, sizeof(seq_t));
        if (cache == NULL)
            return 0;
        for (idx_t i = 1; i <= 9; ++i)
            cache[i] = init_vals[i];
        cached_n = 9;
    }

    if (n <= cached_n)
        return cache[n];

    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (tmp == NULL)
        return 0;
    cache = tmp;

    for (idx_t i = cached_n + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = init_vals[i];
        } else {
            cache[i] = cache[i - 1]
                     + mul921(cache[i - 3])
                     - mul921(cache[i - 4]);
        }
    }
    cached_n = n;
    return cache[n];
}
