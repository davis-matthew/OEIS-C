/*
 * A176966  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=4, k=-1 and l=1.
 * https://oeis.org/A176966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=4. For n>=5, a(n) = ( (7*n-2)*a(n‑1) +
 *            36*(n‑4)*a(n‑4) - ( 3*(n+1)*a(n‑2) + 9*(3*n‑10)*a(n‑3) +
 *            12*(n‑5)*a(n‑5) ) ) / (n+1) This is equivalent to
 *            a(n+1)=∑_{p=0..n}a(p)*a(n–p) – n. It follows the conjectured
 *            order‑6 linear recurrence in the OEIS entry
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  maxn  = 0;

    /* Allocate cache large enough and set initial terms once. */
    if (cache == NULL) {
        size_t need = (size_t)n + 1;
        if (need < 5) need = 5;          /* at least up to a(4) */
        grow_seq(&cache, &cap, need);
        cache[0] = 1;
        if (cap > 1) cache[1] = 4;
        if (cap > 2) cache[2] = 7;
        if (cap > 3) cache[3] = 28;
        if (cap > 4) cache[4] = 109;
        maxn = 4;
    }
    else if (cap <= (size_t)n + 1) {
        size_t newcap = cap * 2;
        if (newcap < (size_t)n + 1) newcap = (size_t)n + 1;
        grow_seq(&cache, &cap, newcap);
    }

    if (n <= maxn)               /* already computed */
        return cache[n];

    for (idx_t i = maxn + 1; i <= n; ++i) {
        if (i < 5)               /* base terms already set */
            continue;

        /* Compute a(i) using the linear recurrence. */
        seq_t t1 = (seq_t)(7 * i - 2) * cache[i - 1];
        seq_t t4 = (seq_t)36 * (i - 4) * cache[i - 4];
        seq_t top = t1 + t4;

        seq_t u1 = (seq_t)3 * (i + 1) * cache[i - 2];
        seq_t u2 = (seq_t)9 * (3 * i - 10) * cache[i - 3];
        seq_t u3 = (seq_t)12 * (i - 5) * cache[i - 5];
        seq_t bottom = u1 + u2 + u3;

        seq_t numerator = top - bottom;
        cache[i] = numerator / (i + 1);
    }

    maxn = n;
    return cache[n];
}
