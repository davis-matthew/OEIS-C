/*
 * A045073  Numbers whose base-4 representation contains exactly two 0's and
 *          two 3's.
 * https://oeis.org/A045073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045073: numbers whose base‑4 representation contains exactly two 0's and two 3's. */
#include <stdlib.h>

static seq_t *cache = NULL;   /* cached sequence values          */
static size_t cap  = 0;       /* current allocation size         */
static size_t sz   = 0;       /* number of values stored         */
static idx_t  last = 0;       /* last integer that has been checked */

static int matches(seq_t x)
{
    seq_t t  = x;
    idx_t cnt0 = 0, cnt3 = 0;
    while (t > 0) {
        seq_t d = t % 4;
        if (d == 0u) {
            if (++cnt0 > 2) return 0;
        } else if (d == 3u) {
            if (++cnt3 > 2) return 0;
        }
        t /= 4;
    }
    return (cnt0 == 2 && cnt3 == 2);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* offset is 1; behaviour below is ignored */
    while (sz < n) {
        ++last;
        if (matches((seq_t)last)) {
            if (sz >= cap) {
                size_t newcap = cap ? cap * 2 : 8;
                seq_t *p = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!p) exit(1);
                cache = p;
                cap   = newcap;
            }
            cache[sz++] = (seq_t)last;
        }
    }
    return cache[n - 1];
}
