/*
 * A022874  a(n) = [ a(n-1)/a(1) ] + [ a(n-1)/a(2) ] + ... + [ a(n-1)/a(n-1) ]
 *          for n >= 3, with initial terms 2,1.
 * https://oeis.org/A022874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n-1} floor(a(n-1)/a(k)) (n>=3) initial values:
 *            a(1)=2, a(2)=1 Note: the recurrence uses only integer operations
 *            on seq_t
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values of the sequence: cache[i] == a(i+1)  */
    static seq_t *cache = NULL;
    static size_t size = 0;      /* number of terms currently stored   */
    static size_t capacity = 0;  /* allocated array capacity (in terms) */

    if (n == 0)                /* Off‑by‑one protection – not used by OEIS */
        return 0;

    /* Ensure enough capacity */
    if ((size_t)n > capacity) {
        size_t new_cap = capacity ? capacity * 2 : 4;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr) abort(); /* memory allocation failed */
        cache = new_arr;
        capacity = new_cap;
    }

    /* Compute missing terms */
    if (size < (size_t)n) {
        /* Initialise the first two terms if needed */
        if (size == 0) {
            cache[0] = (seq_t)2;
            size = 1;
        }
        if (size == 1 && n >= 2) {
            cache[1] = (seq_t)1;
            size = 2;
        }
        /* Compute remaining terms up to n */
        for (size_t i = size; i < (size_t)n; ++i) {
            seq_t prev = cache[i - 1];  /* a(n-1) */
            seq_t sum = 0;
            for (size_t j = 0; j <= i - 1; ++j) {
                sum += prev / cache[j];
            }
            cache[i] = sum;
        }
        size = (size_t)n;
    }

    return cache[n - 1];
}
