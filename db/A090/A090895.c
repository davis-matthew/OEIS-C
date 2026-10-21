/*
 * A090895  a(1)=1 then a(n)=a(n-1)/2 if a(n-1) is even, a(n)=a(n-1)+n
 *          otherwise.
 * https://oeis.org/A090895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1:
      a(n) = a(n-1)/2  if a(n-1) is even,
            = a(n-1)+n  otherwise   */
#include <stddef.h>
#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Ignore calls below the offset (n==0) */
    if (n == 0) return 0;

    /* Create a minimal cache that contains a(1) */
    if (cache_len == 0) {
        cache_len = 1;
        seq_cache = (seq_t *)malloc(2 * sizeof(seq_t));   /* indices 0..1 */
        seq_cache[1] = (seq_t)1;
    }

    /* Expand the cache if we haven't computed up to n yet */
    if (n > cache_len) {
        idx_t old_len = cache_len;
        cache_len = n;
        seq_cache = (seq_t *)realloc(seq_cache, (cache_len + 1) * sizeof(seq_t));

        for (idx_t i = old_len + 1; i <= n; ++i) {
            seq_t prev = seq_cache[i - 1];
            if ((prev & (seq_t)1) == 0) {          /* even */
                seq_cache[i] = prev >> 1;           /* divide by 2 */
            } else {                               /* odd */
                seq_cache[i] = prev + (seq_t)i;
            }
        }
    }

    return seq_cache[n];
}
