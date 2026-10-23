/*
 * A041094  Numerators of continued fraction convergents to sqrt(55).
 * https://oeis.org/A041094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041094: Numerators of continued fraction convergents to sqrt(55).
   Recurrence: a(0)=7; let c(n) = 2 if (n-1)%4 ≠ 3 else 14 (periodic partial quotients
   of sqrt(55)).  Set a(-1)=1.  For n≥1:
        a(n) = c(n) * a(n-1) + a(n-2).  */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;   /* number of elements stored (index+1) */

    /* initialise cache with first term: a(0)=7 */
    if (!cache) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)7;
        cached_len = 1;
    }

    /* if n is already cached, return directly */
    if (n < cached_len) {
        return cache[n];
    }

    /* extend cache up to n (inclusive) */
    seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;

    /* compute missing terms */
    for (idx_t i = cached_len; i <= n; ++i) {
        seq_t ai;
        if (i == 0) {
            ai = (seq_t)7;   /* not used but defined for completeness */
        } else {
            switch ((i - 1) % 4) {
                case 0: case 1: case 2: ai = (seq_t)2; break;
                default:            ai = (seq_t)14; break;
            }
        }
        seq_t val;
        if (i == 1) {
            /* a(1) = c(1)*a(0) + a(-1), where a(-1)=1 */
            val = ai * cache[0] + (seq_t)1;
        } else {
            val = ai * cache[i - 1] + cache[i - 2];
        }
        cache[i] = val;
    }
    cached_len = n + 1;

    return cache[n];
}
