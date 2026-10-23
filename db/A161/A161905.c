/*
 * A161905  a(n) = 13*a(n-1) - 65*a(n-2) + 156*a(n-3) - 182*a(n-4) + 91*a(n-5)
 *          - 13*a(n-6), with a(1)..a(6) as shown.
 * https://oeis.org/A161905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 13*a(n-1) - 65*a(n-2) + 156*a(n-3) - 182*a(n-4) +
 *            91*a(n-5) - 13*a(n-6), with a(1)=2, a(2)=4, a(3)=13, a(4)=52,
 *            a(5)=221, a(6)=949
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 32, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cache of computed terms, 1‑based index */
static idx_t  cache_len = 0;      /* highest index stored in seq_cache */

/* make sure seq_cache contains terms 1..n */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n) return;                /* already have them */
    size_t new_len = (size_t)(n + 1);           /* index 0 unused */
    seq_t *buf = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
    if (!buf) abort();                         /* out of memory */

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        switch (i) {
            case 1: buf[1] = 2;  break;
            case 2: buf[2] = 4;  break;
            case 3: buf[3] = 13; break;
            case 4: buf[4] = 52; break;
            case 5: buf[5] = 221;break;
            case 6: buf[6] = 949;break;
            default:
                buf[i] = 13*buf[i-1] - 65*buf[i-2] + 156*buf[i-3]
                       - 182*buf[i-4] + 91*buf[i-5] - 13*buf[i-6];
        }
    }
    seq_cache = buf;
    cache_len = n;
}

/* public function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* offset is 1, safe guard */
    ensure_cache(n);
    return seq_cache[n];
}
