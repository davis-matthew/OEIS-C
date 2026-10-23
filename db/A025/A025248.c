/*
 * A025248  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ... + a(n-3)*a(3) for n >= 4,
 *          with initial terms 1, 0, 2, 2.
 * https://oeis.org/A025248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=0, a(3)=2, a(4)=2,
   a(n) = sum_{k=1}^{n-3} a(k)*a(n-k)   for n >= 4   */
/* Linear‑recurrence implementation (O(n)): */
/*   n*a(n) +(-2*n+3)*a(n-1)+(n-3)*a(n-2)+4*(-2*n+9)*a(n-3)=0   */

#include <stddef.h>
#include "../utils/grow_seq_array.h"

static seq_t *cache = NULL;   /* 1‑indexed cache of computed terms   */
static idx_t cache_cap = 0;   /* largest index currently stored     */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n < 1, return 0 */

    if (n > cache_cap) {
        /* remember the old maximum index before growing */
        idx_t old_cap = cache_cap;
        /* grow the array to include indices 0 .. n (at least n+1 elements) */
        grow_seq_array(&cache, &cache_cap, (size_t)n + 1);
        cache_cap = n; /* update the maximum valid index */

        /* compute any new terms (from old_cap+1 up to n) */
        for (idx_t i = old_cap + 1; i <= n; ++i) {
            if (i == 1)
                cache[i] = (seq_t)1;
            else if (i == 2)
                cache[i] = (seq_t)0;
            else if (i == 3)
                cache[i] = (seq_t)2;
            else if (i == 4)
                cache[i] = (seq_t)2;
            else {
                /* linear recurrence derived from the
                   conjectured relation:
                   a(n) = [(2n-3)a(n-1) - (n-3)a(n-2) + (8n-36)a(n-3)] / n
                */
                seq_t coeff1 = (seq_t)(2 * i - 3);
                seq_t coeff2 = (seq_t)(i - 3);
                seq_t coeff3 = (seq_t)(8 * i - 36);

                seq_t tmp1 = coeff1 * cache[i - 1];
                seq_t tmp3 = coeff3 * cache[i - 3];
                seq_t tmp2 = coeff2 * cache[i - 2];

                seq_t numerator = (tmp1 + tmp3) - tmp2; /* safe: numerator ≥ 0 */
                cache[i] = numerator / i;               /* exact division */
            }
        }
    }

    return cache[n];
}
