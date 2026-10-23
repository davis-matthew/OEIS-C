/*
 * A033817  Convolution of natural numbers n >= 1 with Lucas numbers L(k) for
 *          k >= -4.
 * https://oeis.org/A033817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Lucas(n-1) + 4*n + 1
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache of Lucas numbers */
static seq_t *lucas_cache = NULL;    /* L(0) .. L(lucas_cap-1) */
static size_t lucas_cap = 0;         /* number of cached entries */

/* Return the n-th term (n >= 1) of OEIS sequence A033817 */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not defined for n < 1 */

    /* ensure cache holds at least n Lucas numbers (indices 0 .. n-1) */
    size_t need = (size_t)n;
    if (lucas_cap < need) {
        size_t new_cap = need;
        seq_t *tmp;

        if (lucas_cache == NULL) {
            /* initial allocation */
            tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!tmp) return 0;
            if (new_cap >= 1) tmp[0] = (seq_t)2;   /* L(0) */
            if (new_cap >= 2) tmp[1] = (seq_t)1;   /* L(1) */
            for (size_t i = 2; i < new_cap; ++i) {
                tmp[i] = tmp[i-1] + tmp[i-2];    /* L(i) */
            }
        } else {
            /* extend existing cache */
            tmp = (seq_t *)realloc(lucas_cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;
            for (size_t i = lucas_cap; i < new_cap; ++i) {
                if (i == 0)          tmp[0] = (seq_t)2;
                else if (i == 1)      tmp[1] = (seq_t)1;
                else                  tmp[i] = tmp[i-1] + tmp[i-2];
            }
        }
        lucas_cache = tmp;
        lucas_cap = new_cap;
    }

    /* a(n) = L(n-1) + 4*n + 1 */
    seq_t lucas_val = lucas_cache[need - 1];
    seq_t result = lucas_val + ((seq_t)4 * (seq_t)n) + (seq_t)1;
    return result;
}
