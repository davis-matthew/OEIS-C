/*
 * A180926  Numbers k such that 6*k and 10*k are triangular numbers.
 * https://oeis.org/A180926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (62*a(n-1) + 1 + sqrt((48*a(n-1)+1)*(80*a(n-1)+1)))/2,
 *            a(1)=0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 456 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* memoised recurrence computation of A180926 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of computed values */
    static size_t cap   = 0;          /* allocated capacity */
    static idx_t  sz    = 0;          /* number of values stored */

    if (n < 1)
        return 0;                    /* undefined for n<1, return 0 */

    /* grow cache to contain at least n elements */
    if ((size_t)n > cap) {
        size_t newcap = (cap == 0) ? 4 : cap * 2;
        while (newcap < n)
            newcap *= 2;
        seq_t *tmp = mem_resize(seq_t, cache, newcap);
        if (!tmp)                 /* allocation failed – not expected */
            return 0;
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms up to n */
    for (idx_t i = sz + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = 0;                     /* a(1) = 0 */
        } else {
            seq_t prev = cache[i - 2];         /* a(i-1) */

            /* square roots of (48*a + 1) and (80*a + 1) – they are perfect squares */
            seq_t sqrtA = floor_sqrt((seq_t)48 * prev + 1);
            seq_t sqrtB = floor_sqrt((seq_t)80 * prev + 1);

            /* product of the two square roots */
            seq_t prod = sqrtA * sqrtB;

            /* new term via the recurrence */
            seq_t next = ((seq_t)62 * prev + 1 + prod) / 2;
            cache[i - 1] = next;
        }
    }

    sz = n;
    return cache[n - 1];
}
