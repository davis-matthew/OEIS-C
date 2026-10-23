/*
 * A125319  Number of base 9 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: a(n) = number of base 9 circular n‑digit numbers
 * with adjacent digits differing by at most 3.
 *
 * Recurrence (for n ≥ 8):
 *   a(n) = 10*a(n-1) - 25*a(n-2) + 34*a(n-4)
 *            - 14*a(n-5) - 8*a(n-6) + 4*a(n-7)
 *
 * Initial terms (n = 0 … 7):
 *   a(0)=1   a(1)=9   a(2)=51   a(3)=249
 *   a(4)=1387 a(5)=7879 a(6)=45663 a(7)=267367
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Resize cache if necessary */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        /* Compute missing terms */
        for (size_t k = cache_size; k <= n; ++k) {
            seq_t val;
            if (k == 0)      val = (seq_t)1;
            else if (k == 1) val = (seq_t)9;
            else if (k == 2) val = (seq_t)51;
            else if (k == 3) val = (seq_t)249;
            else if (k == 4) val = (seq_t)1387;
            else if (k == 5) val = (seq_t)7879;
            else if (k == 6) val = (seq_t)45663;
            else if (k == 7) val = (seq_t)267367;
            else
                /* recurrence: based on previous 7 values */
                val =  (seq_t)10 * cache[k-1]
                     - (seq_t)25 * cache[k-2]
                     + (seq_t)34 * cache[k-4]
                     - (seq_t)14 * cache[k-5]
                     - (seq_t)8  * cache[k-6]
                     + (seq_t)4  * cache[k-7];
            cache[k] = val;
        }
        cache_size = new_size;
    }
    return cache[n];
}
