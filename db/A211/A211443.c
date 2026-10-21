/*
 * A211443  Number of (n+1)X(n+1) -7..7 symmetric matrices with every 2X2
 *          subblock having sum zero and one or three distinct values.
 * https://oeis.org/A211443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            3*a(n-1)+9*a(n-2)-31*a(n-3)-30*a(n-4)+125*a(n-5)+48*a(n-6) *
 *            -251*a(n-7)-46*a(n-8)+264*a(n-9)+40*a(n-10)-138*a(n-11) *
 *            -28*a(n-12)+28*a(n-13)+8*a(n-14) * Initial values: * a(1)=91,
 *            a(2)=201, a(3)=397, a(4)=765, a(5)=1459, a(6)=2751, * a(7)=5215,
 *            a(8)=9855, a(9)=18759, a(10)=35741, a(11)=68...
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[14] = {
    91, 201, 397, 765, 1459, 2751,
    5215, 9855, 18759, 35741, 68529,
    131739, 254515, 493209
};

static seq_t *cache   = NULL;
static size_t  cache_size = 0;
static size_t  cache_cap   = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not used, but harmless */

    /* For n up to 14 we can return directly from the initial table. */
    if (n <= 14) return init[n - 1];

    /* Initialise cache if this is the first call. */
    if (cache == NULL) {
        cache_cap = 16; /* enough to hold the first 14 terms and grow. */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (int i = 0; i < 14; ++i) cache[i] = init[i];
        cache_size = 14;
    }

    /* Compute all terms up to n, storing them for later calls. */
    while (cache_size < n) {
        if (cache_size >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* Retrieve the last 14 values needed for the recurrence. */
        seq_t t1  = cache[cache_size - 1];
        seq_t t2  = cache[cache_size - 2];
        seq_t t3  = cache[cache_size - 3];
        seq_t t4  = cache[cache_size - 4];
        seq_t t5  = cache[cache_size - 5];
        seq_t t6  = cache[cache_size - 6];
        seq_t t7  = cache[cache_size - 7];
        seq_t t8  = cache[cache_size - 8];
        seq_t t9  = cache[cache_size - 9];
        seq_t t10 = cache[cache_size -10];
        seq_t t11 = cache[cache_size -11];
        seq_t t12 = cache[cache_size -12];
        seq_t t13 = cache[cache_size -13];
        seq_t t14 = cache[cache_size -14];

        /* Separate positive and negative contributions to avoid unsigned underflow. */
        seq_t sumPos = (seq_t)3 * t1 + (seq_t)9 * t2
                       + (seq_t)125 * t5 + (seq_t)48 * t6
                       + (seq_t)264 * t9 + (seq_t)40 * t10
                       + (seq_t)28  * t13 + (seq_t)8 * t14;

        seq_t sumNeg = (seq_t)31 * t3 + (seq_t)30 * t4
                       + (seq_t)251 * t7 + (seq_t)46 * t8
                       + (seq_t)138 * t11 + (seq_t)28 * t12;

        seq_t next = sumPos - sumNeg;   /* guaranteed non‑negative */
        cache[cache_size] = next;
        ++cache_size;
    }

    return cache[n - 1];
}
