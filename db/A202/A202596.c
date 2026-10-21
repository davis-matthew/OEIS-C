/*
 * A202596  Number of (n+2)X5 binary arrays avoiding patterns 001 and 000 in
 *          rows and columns.
 * https://oeis.org/A202596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 43*a(n-2) + 16*a(n-3) - 169*a(n-4) - 8*a(n-5)
 *            + 161*a(n-6) - 16*a(n-7) - 31*a(n-8) + 4*a(n-9) + a(n-10);
 *            Initial values: a(1)=1968, a(2)=18160, a(3)=157952,
 *            a(4)=1402544, a(5)=12360432, a(6)=109234112, a(7)=964348880,
 *            a(8)=8516820112, a(9)=75206974464, a(10)=664144023888
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_terms[10] = {
    1968,
    18160,
    157952,
    1402544,
    12360432,
    109234112,
    964348880,
    8516820112ULL,
    75206974464ULL,
    664144023888ULL
};

static seq_t *cached = NULL;
static idx_t cached_size = 0;
static idx_t cached_used = 0;

static void ensure_cache(idx_t n)
{
    if (cached_used >= n)
        return;
    if (n > cached_size) {
        seq_t *tmp = (seq_t *)realloc(cached, n * sizeof(seq_t));
        if (!tmp && n > 0)
            return; /* allocation failed – leave unchanged */
        cached = tmp;
        cached_size = n;
    }
    for (idx_t i = cached_used + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 10) {
            val = init_terms[i - 1];
        } else {
            /* indices of previous terms relative to current i */
            idx_t i1 = i - 2;   /* a(i-1) */
            idx_t i2 = i - 3;   /* a(i-2) */
            idx_t i3 = i - 4;   /* a(i-3) */
            idx_t i4 = i - 5;   /* a(i-4) */
            idx_t i5 = i - 6;   /* a(i-5) */
            idx_t i6 = i - 7;   /* a(i-6) */
            idx_t i7 = i - 8;   /* a(i-7) */
            idx_t i8 = i - 9;   /* a(i-8) */
            idx_t i9 = i -10;   /* a(i-9) */
            idx_t i10= i -11;   /* a(i-10) */

            seq_t pos = 0;
            pos += 4   * cached[i1];
            pos += 43  * cached[i2];
            pos += 16  * cached[i3];
            pos += 161 * cached[i6];
            pos += 4   * cached[i9];
            pos +=     cached[i10];

            seq_t neg = 0;
            neg += 169 * cached[i4];
            neg += 8   * cached[i5];
            neg += 16  * cached[i7];
            neg += 31  * cached[i8];

            val = pos - neg;
        }
        cached[i - 1] = val;
    }
    cached_used = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined for n=0, return 0 as placeholder */
    ensure_cache(n);
    return cached[n - 1];
}
