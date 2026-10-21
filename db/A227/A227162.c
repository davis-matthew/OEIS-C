/*
 * A227162  Number of n X 3 0,1 arrays indicating 2 X 2 subblocks of some
 *          larger (n+1) X 4 binary array having a sum of one or less, with
 *          rows and columns of the latter in lexicographically nondecreasing
 *          order.
 * https://oeis.org/A227162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 40, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Title: A227162
   Formula: a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) - 210*a(n-4)
            + 252*a(n-5) - 210*a(n-6) + 120*a(n-7) - 45*a(n-8)
            + 10*a(n-9) - a(n-10)      (for n > 13)
   Initial values: a(1)=4, a(2)=18, a(3)=62, a(4)=193, a(5)=558,
                    a(6)=1507, a(7)=3828, a(8)=9149, a(9)=20609,
                    a(10)=43918, a(11)=88960, a(12)=172130,
                    a(13)=319637
   This implementation memoises the sequence in a static array.
*/

#include <stddef.h>

static const seq_t init_vals[13] = {
    (seq_t)4,    (seq_t)18,    (seq_t)62,    (seq_t)193,    (seq_t)558,
    (seq_t)1507, (seq_t)3828,  (seq_t)9149,  (seq_t)20609,  (seq_t)43918,
    (seq_t)88960, (seq_t)172130,(seq_t)319637
};

/* cache size sufficient for the typical test range (up to about 10 000) */
#define MAX_N 20000
static seq_t cache[MAX_N + 1];      /* 1‑based indexing; cache[0] unused */
static idx_t cached_to = 0;          /* largest n for which cache[n] is valid */

/* ------------------------------------------------------------ */
/* Return the n-th term of sequence A227162 (n >= 1).            */
/* All arithmetic is performed in the type seq_t; for n <= 13    */
/* the exact initial values are returned.                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return 0;   /* undefined for n < 1, return 0 for safety */

    if (n <= (idx_t)13)
        return init_vals[n - 1];

    /* initialise cache with first 13 values if necessary */
    if (cached_to < (idx_t)13) {
        for (idx_t i = (idx_t)1; i <= (idx_t)13; ++i)
            cache[i] = init_vals[i - 1];
        cached_to = (idx_t)13;
    }

    /* expand cache up to required n (within the allocated limit) */
    if (n > (idx_t)MAX_N) {
        /* For builds where n may exceed MAX_N, compute on demand without caching */
        seq_t val = (seq_t)0;
        /* Temporarily compute using a sliding window of the last 10 values */
        seq_t last[10] = {0};
        /* initialise last[0..9] with the first 13 values for the recurrence */
        for (idx_t i = 0; i < 13; ++i)
            last[i % 10] = init_vals[i];
        /* now iterate from 14 up to n */
        for (idx_t i = 14; i <= n; ++i) {
            val = (seq_t)10 * last[(i - 1) % 10];
            val -= (seq_t)45 * last[(i - 2) % 10];
            val += (seq_t)120 * last[(i - 3) % 10];
            val -= (seq_t)210 * last[(i - 4) % 10];
            val += (seq_t)252 * last[(i - 5) % 10];
            val -= (seq_t)210 * last[(i - 6) % 10];
            val += (seq_t)120 * last[(i - 7) % 10];
            val -= (seq_t)45 * last[(i - 8) % 10];
            val += (seq_t)10 * last[(i - 9) % 10];
            val -= last[(i - 10) % 10];
            /* shift window */
            last[i % 10] = val;
        }
        return val;
    }

    if (n > cached_to) {
        for (idx_t i = cached_to + (idx_t)1; i <= n; ++i) {
            seq_t val = (seq_t)10 * cache[i - 1];
            val -= (seq_t)45 * cache[i - 2];
            val += (seq_t)120 * cache[i - 3];
            val -= (seq_t)210 * cache[i - 4];
            val += (seq_t)252 * cache[i - 5];
            val -= (seq_t)210 * cache[i - 6];
            val += (seq_t)120 * cache[i - 7];
            val -= (seq_t)45 * cache[i - 8];
            val += (seq_t)10 * cache[i - 9];
            val -= cache[i - 10];
            cache[i] = val;
        }
        cached_to = n;
    }

    return cache[n];
}
