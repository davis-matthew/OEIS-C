/*
 * A099423  Lean quaternary temporal logic [LQTL] cumulative column
 *          frequencies of LQTL logic in A094266.
 * https://oeis.org/A099423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-4) - 10*a(n-8) + 10*a(n-12) - 4*a(n-16) with
 *            initial values a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=2, a(5)=1,
 *            a(6)=0, a(7)=0, a(8)=3, a(9)=3, a(10)=1, a(11)=0, a(12)=4,
 *            a(13)=6, a(14)=4, a(15)=1
 * Range:     u8 n <= 35, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n) {
    /* static cache big enough for all tests (~10 000); enlarge if needed   */
    enum { CACHE_SIZE = 20000 };          /* adjust if larger n are used   */
    static seq_t cache[CACHE_SIZE + 1];
    static size_t cache_len = 0;          /* first index that is cached   */

    /* initialise the cache on the first call */
    if (cache_len == 0) {
        /* zero out all entries (trivial for built‑in types, default ctor
           for big integer types)                                  */
        for (size_t i = 0; i <= CACHE_SIZE; ++i)
            cache[i] = (seq_t)0;

        /* fill the known initial 16 terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)1;
        cache[6] = (seq_t)0;
        cache[7] = (seq_t)0;
        cache[8] = (seq_t)3;
        cache[9] = (seq_t)3;
        cache[10] = (seq_t)1;
        cache[11] = (seq_t)0;
        cache[12] = (seq_t)4;
        cache[13] = (seq_t)6;
        cache[14] = (seq_t)4;
        cache[15] = (seq_t)1;

        cache_len = 16;
    }

    /* extend the cache if the requested index is beyond what we know   */
    if ((size_t)n >= cache_len) {
        /* compute new values in order, using the recurrence              */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            /* for i < 16 the values are already initialised                 */
            if (i < 16) continue;

            /* compute the recurrence using only seq_t operations          */
            seq_t pos = 5 * cache[i - 4] + 10 * cache[i - 12];
            seq_t neg = 10 * cache[i - 8] + 4 * cache[i - 16];
            cache[i] = pos - neg;   /* guaranteed non‑negative              */
        }
        cache_len = n + 1;
    }

    return cache[(idx_t)n];
}
