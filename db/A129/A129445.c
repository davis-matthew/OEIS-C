/*
 * A129445  Numbers k > 0 such that k^2 is a centered triangular number.
 * https://oeis.org/A129445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-2) - a(n-4) (a(1)=1, a(2)=2, a(3)=8, a(4)=19)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL; /* stores a(1)…a(seq_len)  */
static idx_t   seq_len    = 0;   /* highest index computed so far      */
static size_t  seq_cap    = 0;   /* allocated capacity (array size)    */

static void seq_ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < (size_t)n + 1) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    seq_cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1; 0 is unused        */
    seq_ensure_capacity(n);

    if (seq_len >= n) return seq_cache[n];

    for (idx_t i = seq_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)          val = 1;
        else if (i == 2)     val = 2;
        else if (i == 3)     val = 8;
        else if (i == 4)     val = 19;
        else                 /* recurrence: a(i) = 10*a(i-2) - a(i-4) */
            val = seq_cache[i - 2] * (seq_t)10 - seq_cache[i - 4];
        seq_cache[i] = val;
    }
    seq_len = n;
    return seq_cache[n];
}
