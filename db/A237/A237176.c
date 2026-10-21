/*
 * A237176  Number of (n+1)X(1+1) 0..2 arrays colored with the maximum plus
 *          the minimum minus the lower median of every 2X2 subblock.
 * https://oeis.org/A237176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A237176
   a(n) = 32*a(n-3) + 288*a(n-4) + 464*a(n-5) + 2632*a(n-6) + 2144*a(n-7)
          + 6720*a(n-8) + 3624*a(n-9) - 10064*a(n-10) - 96*a(n-11) - 52480*a(n-12)
          - 13312*a(n-13) - 1536*a(n-14) - 16384*a(n-15) + 81920*a(n-16)
   with a(1)=81,a(2)=402,a(3)=2061,a(4)=10488,a(5)=53401,a(6)=271420,
   a(7)=1382012,a(8)=7030000,a(9)=35777132,a(10)=182015816,a(11)=926277176,
   a(12)=4712685384,a(13)=23981258640,a(14)=122017499720,
   a(15)=620885520496,a(16)=3159152802944.  */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_INDEX 200000   /* sufficient for all tested values */

static seq_t cache[MAX_INDEX + 1];
static bool initialized = false;
static idx_t last_cached = 0;

/* helper to set base values on first use */
static inline void init_cache(void)
{
    if (initialized)
        return;
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)81;
    cache[2] = (seq_t)402;
    cache[3] = (seq_t)2061;
    cache[4] = (seq_t)10488;
    cache[5] = (seq_t)53401;
    cache[6] = (seq_t)271420;
    cache[7] = (seq_t)1382012;
    cache[8] = (seq_t)7030000;
    cache[9] = (seq_t)35777132;
    cache[10] = (seq_t)182015816;
    cache[11] = (seq_t)926277176;
    cache[12] = (seq_t)4712685384;
    cache[13] = (seq_t)23981258640;
    cache[14] = (seq_t)122017499720;
    cache[15] = (seq_t)620885520496;
    cache[16] = (seq_t)3159152802944;
    last_cached = 16;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but return 0 for safety */

    init_cache();

    if (n <= last_cached)
        return cache[(size_t)n];

    /* compute missing terms up to n */
    for (idx_t i = last_cached + 1; i <= n; ++i) {
        /* positive part */
        seq_t pos =
            cache[(size_t)(i - 3)] * ((seq_t)32)  +
            cache[(size_t)(i - 4)] * ((seq_t)288) +
            cache[(size_t)(i - 5)] * ((seq_t)464)  +
            cache[(size_t)(i - 6)] * ((seq_t)2632) +
            cache[(size_t)(i - 7)] * ((seq_t)2144) +
            cache[(size_t)(i - 8)] * ((seq_t)6720) +
            cache[(size_t)(i - 9)] * ((seq_t)3624) +
            cache[(size_t)(i -16)] * ((seq_t)81920);
        /* negative part */
        seq_t neg =
            cache[(size_t)(i -10)] * ((seq_t)10064) +
            cache[(size_t)(i -11)] * ((seq_t)96)    +
            cache[(size_t)(i -12)] * ((seq_t)52480) +
            cache[(size_t)(i -13)] * ((seq_t)13312) +
            cache[(size_t)(i -14)] * ((seq_t)1536)  +
            cache[(size_t)(i -15)] * ((seq_t)16384);
        cache[(size_t)i] = pos - neg;
    }
    last_cached = n;
    return cache[(size_t)n];
}
