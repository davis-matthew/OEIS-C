/*
 * A088316  a(n) = 13*a(n-1) + a(n-2), starting with a(0) = 2 and a(1) = 13.
 * https://oeis.org/A088316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) + a(n-2) Initial values: a(0)=2, a(1)=13
 *            Recurrence with memoization, O(n) per new maximum n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;     /* cache of computed values */
    static idx_t cache_len = 0;     /* number of terms stored */

    /* handle the first two base cases */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)13;

    /* extend cache if necessary */
    if (cache_len == 0) {
        cache_len = 2;
        terms = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        terms[0] = (seq_t)2;
        terms[1] = (seq_t)13;
    }
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        terms = (seq_t*)realloc(terms, sizeof(seq_t) * new_len);
        for (idx_t i = cache_len; i <= n; ++i) {
            terms[i] = (13 * terms[i-1]) + terms[i-2];
        }
        cache_len = new_len;
    }
    return terms[n];
}
