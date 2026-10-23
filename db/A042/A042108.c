/*
 * A042108  Numerators of continued fraction convergents to sqrt(579).
 * https://oeis.org/A042108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 770*a(n−2) – a(n−4),   offset = 0
   Initial values: a(0)=24, a(1)=385, a(2)=18504, a(3)=296449   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stored terms a(0)..a(cache_len‑1)      */
    static idx_t cache_len = 0;   /* number of terms currently stored      */
    idx_t required = n + 1;       /* we must have size >= required         */

    if (required > cache_len) {
        /* Allocate a larger buffer, copy old terms, free old buffer.     */
        seq_t *new_cache;
#ifdef __cplusplus
        /* C++ path: use operator new[] and delete[].                    */
        new_cache = new seq_t[required];
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
#else
        /* C path: use realloc (safe for POD types, valid for the tests). */
        new_cache = (seq_t*)realloc(cache, required * sizeof(seq_t));
        /* realloc preserves existing values. */
#endif
        cache = new_cache;

        /* Initialize the first four known terms if the cache is newly
           allocated or truncated below 4.                               */
        if (required > 0)
            cache[0] = (seq_t)24;
        if (required > 1)
            cache[1] = (seq_t)385;
        if (required > 2)
            cache[2] = (seq_t)18504;
        if (required > 3)
            cache[3] = (seq_t)296449;

        /* Compute further terms using the recurrence.  We start from
           the first index that is not yet computed.                     */
        idx_t start = (cache_len < 4) ? (idx_t)4 : cache_len;
        for (idx_t i = start; i < required; ++i) {
            cache[i] = cache[i - 2] * (seq_t)770 - cache[i - 4];
        }
        cache_len = required;
    }

    return cache[n];
}
