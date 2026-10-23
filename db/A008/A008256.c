/*
 * A008256  Coordination sequence T1 for Milarite.
 * https://oeis.org/A008256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-5)+a(n-6)+a(n-8)-a(n-11)-a(n-13)-a(n-14)+a(n-19) for
 *            n ≥ 22, with initial values
 *            a(0)=1,a(1)=4,a(2)=10,a(3)=22,a(4)=43,
 *            a(5)=69,a(6)=95,a(7)=123,a(8)=162,a(9)=215,a(10)=269,a(11)=319,
 *            a(12)=372,a(13)=435,a(14)=513,a(15)=597,a(16)=675,a(17)=754,
 *            a(18)=843,a(19)=943,a(20)=1052,a(21)=1162
 * Range:     u8 n <= 9, u16 n <= 158, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* The build supplies seq_t and idx_t. */
static const seq_t init_vals[22] = {
    1ULL, 4ULL, 10ULL, 22ULL, 43ULL, 69ULL, 95ULL, 123ULL, 162ULL, 215ULL,
    269ULL, 319ULL, 372ULL, 435ULL, 513ULL, 597ULL, 675ULL, 754ULL, 843ULL,
    943ULL, 1052ULL, 1162ULL
};

/* Prefixed cache of computed terms.  We keep a static array large
   enough for the typical maximal test (n≈10000).  The array is
   initialized once with the 22 known terms. */
#define CACHE_MAX 20000
static seq_t cache[CACHE_MAX];
static size_t cache_len = 0;  /* the first unused index of cache */

static void init_cache(void)
{
    if (cache_len != 0) return;   /* already initialized */
    for (size_t i = 0; i < 22; ++i) cache[i] = init_vals[i];
    cache_len = 22;
}

seq_t a(idx_t n)
{
    init_cache();                      /* ensure cache starts filled */

    /* Return initial known values directly. */
    if (n < 22) return cache[(size_t)n];

    /* Compute terms up to n, using the recurrence.  The loop will be
       entered only once per increasing n, but the function works
       regardless of the order of calls because cache_len keeps all
       previously computed values. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val = 0;
        if (i >= 5)  val += cache[i - 5];
        if (i >= 6)  val += cache[i - 6];
        if (i >= 8)  val += cache[i - 8];
        if (i >= 11) val -= cache[i - 11];
        if (i >= 13) val -= cache[i - 13];
        if (i >= 14) val -= cache[i - 14];
        if (i >= 19) val += cache[i - 19];
        cache[i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
