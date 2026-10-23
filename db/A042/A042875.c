/*
 * A042875  Denominators of continued fraction convergents to sqrt(969).
 * https://oeis.org/A042875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 21, u64 n <= 44, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A042875 – denominators of the continued‑fraction convergents to √969. */
/* Recurrence (used implicitly via periodic continued fraction):
   a(0)=1, a(1)=7, a(2)=8, a(3)=31, a(4)=101, a(5)=132, a(6)=233,
   a(7)=365, a(8)=963, a(9)=1328, a(10)=2291, a(11)=3619, a(12)=13148,
   a(13)=43063, a(14)=56211, a(15)=436540,
   a(n)=27177902*a(n-16)-a(n-32) for n>31.                  */

#include <stdlib.h>

/*
 * The convergents are generated with the recurrences
 *   p_k = a_k * p_(k-1) + p_(k-2)
 *   q_k = a_k * q_(k-1) + q_(k-2)
 * where the first partial quotient of √969 (31) is omitted.
 * After the initial q(–1)=0 and q(0)=1, the repeating partial
 * quotients are:
 *   [7,1,3,3,1,1,1,2,1,1,1,3,3,1,7,62].
 */
seq_t a(idx_t n)
{
    /* Static cache of previously computed denominators q_k. */
    static seq_t *cache      = NULL;   /* array of q_k values          */
    static size_t cache_len  = 0;      /* number of stored terms usable  */
    /* Periodic partial quotients beyond the first term. */
    static const seq_t period[16] = {
        (seq_t)7,  (seq_t)1,  (seq_t)3,  (seq_t)3,
        (seq_t)1,  (seq_t)1,  (seq_t)1,  (seq_t)2,
        (seq_t)1,  (seq_t)1,  (seq_t)1,  (seq_t)3,
        (seq_t)3,  (seq_t)1,  (seq_t)7,  (seq_t)62
    };

    /* Initialise cache with q0 = 1 if it hasn't been done yet. */
    if (cache_len == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;              /* allocation failure */
        cache[0] = (seq_t)1;                     /* q0 = 1   */
        cache_len = 1;
    }

    /* Extend the cache so that it contains all q_k up to index n. */
    size_t required = (size_t)n + 1;
    if (required > cache_len) {
        seq_t *tmp = (seq_t*)realloc(cache, required * sizeof(seq_t));
        if (!tmp) return (seq_t)0;               /* allocation failure */
        cache = tmp;
        /* The two most recent terms before the new block. */
        seq_t prevprev = (cache_len >= 2) ? cache[cache_len - 2] : (seq_t)0;
        seq_t prev     = cache[cache_len - 1];

        for (size_t k = cache_len; k < required; ++k) {
            /* a_k is indexed from 1; for k>=1 use period[(k-1) & 15]. */
            seq_t coeff  = period[(k - 1) & 15];
            seq_t next   = coeff * prev + prevprev;
            cache[k] = next;
            prevprev  = prev;
            prev      = next;
        }
        cache_len = required;
    }

    return cache[(size_t)n];
}
