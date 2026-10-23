/*
 * A163502  a(n) is the smallest number which is divisible by n, is not equal
 *          to n and its digital sum is also divisible by n.
 * https://oeis.org/A163502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 39, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A163502
   a(n): the smallest m > n such that m % n == 0 and digit_sum(m) % n == 0
   implemented by brute‑force search.  The function memoises the results
   across calls in a static array for efficiency.  It uses the util
   function seq_t digit_sum(seq_t v). */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"

static seq_t *cache = NULL;       /* cache[n-1] holds a(n) for n>=1 */
static idx_t cache_size = 0;      /* number of elements currently stored */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset starts at 1; value for 0 is undefined */
    /* extend cache if necessary */
    if (n > cache_size) {
        size_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp && new_size > 0) { /* memory failure – fall back to one‑by‑one */
            tmp = cache;
        } else {
            cache = tmp;
        }
        /* compute values for all indices up to n that are not yet cached */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t N = (seq_t)i;
            seq_t k = 2;                 /* start from 2*n to guarantee m > n */
            seq_t result = 0;
            while (1) {
                seq_t m = N * k;
                if (digit_sum(m) % N == 0) {
                    result = m;
                    break;
                }
                ++k;
            }
            cache[i-1] = result;
        }
        cache_size = n;
    }
    return cache[n-1];
}
