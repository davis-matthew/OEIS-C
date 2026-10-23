/*
 * A118635  Start with 1 and repeatedly reverse the digits and add 39 to get
 *          the next term.
 * https://oeis.org/A118635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A118635
   a(1) = 1
   a(n) = reverse(a(n-1)) + 39   (for n > 1)  */
#include <stdlib.h>
#include <string.h>
#include "../utils/reverse_digits.h"   /* seq_t reverse_digits(seq_t) */

seq_t a(idx_t n)
{
    /* static cache: a(1) ... a(size) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;     /* allocated capacity in elements   */
    static idx_t  size  = 0;     /* number of terms stored (>=0)     */

    if (n < 1) return 0;         /* out of range – callers assume n>=1 */

    /* grow cache if needed */
    if (n > size) {
        if (cap < (size_t)n) {
            size_t new_cap = cap ? cap << 1 : 1;       /* double or start at 1 */
            if (new_cap < (size_t)n) new_cap = (size_t)n;
            seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (!tmp) exit(1);   /* allocation failed */
            if (cache) {
                memcpy(tmp, cache, size * sizeof(seq_t));
                free(cache);
            }
            cache = tmp;
            cap   = new_cap;
        }

        /* compute missing terms up to n */
        for (idx_t i = size + 1; i <= n; ++i) {
            seq_t next;
            if (size == 0) {
                next = (seq_t)1;   /* a(1) = 1 */
            } else {
                next = reverse_digits(cache[size - 1]) + (seq_t)39;
            }
            cache[size] = next;
            ++size;
        }
    }

    return cache[n - 1];
}
