/*
 * A212903  Number of (w,x,y,z) with all terms in {0,...,n} and
 *          |w-x|<=|x-y|<=|y-z|.
 * https://oeis.org/A212903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            2*a(n-1)+a(n-2)-3*a(n-3)-a(n-4)+a(n-5)+3*a(n-6)-a(n-7)-2*a(n-8)+a(n-9)
 *            with initial values: a(0)=1, a(1)=8, a(2)=35, a(3)=94, a(4)=209,
 *            a(5)=398, a(6)=697, a(7)=1130, a(8)=1743
 * Range:     u8 n <= 4, u16 n <= 22, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if (cache_cap > n)
        return;

    size_t old_cap = cache_cap;
    cache_cap = n + 1;
    seq_cache = (seq_t*)realloc(seq_cache, cache_cap * sizeof(seq_t));
    /* initialize first nine values if they are not yet set */
    static const seq_t init[9] = {
        (seq_t)1, (seq_t)8, (seq_t)35, (seq_t)94, (seq_t)209,
        (seq_t)398, (seq_t)697, (seq_t)1130, (seq_t)1743
    };
    for (idx_t i = old_cap; i <= n; ++i) {
        if (i <= 8) {
            seq_cache[i] = init[i];
        } else {
            /* Compute a(n) using the recurrence */
            seq_t a1 = seq_cache[i-1];
            seq_t a2 = seq_cache[i-2];
            seq_t a3 = seq_cache[i-3];
            seq_t a4 = seq_cache[i-4];
            seq_t a5 = seq_cache[i-5];
            seq_t a6 = seq_cache[i-6];
            seq_t a7 = seq_cache[i-7];
            seq_t a8 = seq_cache[i-8];
            seq_t a9 = seq_cache[i-9];

            /* Arrange positives first to avoid unsigned underflow */
            seq_t pos = 2*a1 + a2 + a5 + a9 + 3*a6;
            seq_t neg = 3*a3 + a4 + a7 + 2*a8;
            seq_cache[i] = pos - neg;
        }
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[n];
}
