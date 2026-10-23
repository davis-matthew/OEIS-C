/*
 * A115269  Row sums of correlation triangle for floor((n+4)/4).
 * https://oeis.org/A115269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 78, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115269: Row sums of correlation triangle for floor((n+4)/4)
   Recurrence (minimal, order 12):
     a(n) = 2*a(n-1) – 2*a(n-3) + 3*a(n-4) – 4*a(n-5)
            + 4*a(n-7) – 3*a(n-8) + 2*a(n-9) – 2*a(n-11) + a(n-12)
   Initial values:
     a(0)=1, a(1)=2, a(2)=4, a(3)=6, a(4)=11,
     a(5)=16, a(6)=24, a(7)=32, a(8)=46, a(9)=60,
     a(10)=80, a(11)=100
   The routine memoises computed terms so that calls in arbitrary
   order are O(1) for each new index. */

#include <stddef.h>
#include <stdlib.h>

/* Grow dynamic array up to at least new_len elements */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len)
        return;
    size_t new_size = new_len;
    seq_t *tmp = (seq_t *)realloc(*arr, new_size * sizeof(seq_t));
    if (!tmp)
        abort();          /* out of memory */
    *arr   = tmp;
    *size  = new_size;
}

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;    /* allocated capacity */
    static idx_t len    = 0;    /* next free index (i.e. cache size) */

    /* initialise first 12 terms once */
    if (len == 0) {
        grow_seq_array(&cache, &cap, 12);
        cache[0] = 1;  cache[1] = 2;  cache[2] = 4;  cache[3] = 6;
        cache[4] = 11; cache[5] = 16; cache[6] = 24; cache[7] = 32;
        cache[8] = 46; cache[9] = 60; cache[10] = 80; cache[11] = 100;
        len = 12;
    }

    if (n < len)
        return cache[n];

    /* make sure we have storage for indices up to n */
    grow_seq_array(&cache, &cap, n + 1);

    /* compute new terms iteratively */
    for (idx_t i = len; i <= n; ++i) {
        /* use signed intermediate to avoid unsigned subtraction wrap */
        long long val = 0;
        if (i >= 1)
            val += 2LL * (long long)cache[i - 1];
        if (i >= 3)
            val -= 2LL * (long long)cache[i - 3];
        if (i >= 4)
            val += 3LL * (long long)cache[i - 4];
        if (i >= 5)
            val -= 4LL * (long long)cache[i - 5];
        if (i >= 7)
            val += 4LL * (long long)cache[i - 7];
        if (i >= 8)
            val -= 3LL * (long long)cache[i - 8];
        if (i >= 9)
            val += 2LL * (long long)cache[i - 9];
        if (i >= 11)
            val -= 2LL * (long long)cache[i - 11];
        if (i >= 12)
            val += (long long)cache[i - 12];

        cache[i] = (seq_t)val;   /* cast back to seq_t (truncates if needed) */
    }

    len = n + 1;
    return cache[n];
}
